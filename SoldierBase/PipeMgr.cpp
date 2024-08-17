#include "pch.h"
#include "PipeMgr.h"
#include "PipeServer.h"
#include "InternalIOContext.h"
#include "PipeSessionServer.h"
#include "PipeSessionClient.h"

smart_result _stdcall PipeMgr::CreatePipeServer(const wchar_t* pipe_name, IPipeServerCallback* server_callback)
{
    if (!pipe_name || !server_callback)
        return err_code::e_invalidarg;

    {
        WriteLock lock(list_lock_);
        auto iter = server_list_.find(pipe_name);

        if (iter == server_list_.end())
        {
            IPipeServerPtr server;
            PipeServer::CreateInstanceEx(server, pipe_name);
            server_list_.insert({ pipe_name, server });
        }
    }
    
    return InternalCreateServer(pipe_name, server_callback);
}

smart_result _stdcall PipeMgr::GetServer(const wchar_t* pipe_name, IPipeServer** server)
{
    if (!pipe_name || !server)return err_code::e_invalidarg;

    ReadLock lock(list_lock_);

    auto iter = server_list_.find(pipe_name);
    if (iter == server_list_.end())return err_code::e_fail;

    (*server) = iter->second;
    (*server)->addref();

    return err_code::error_success;
}

smart_result _stdcall PipeMgr::CloseServer(const wchar_t* pipe_name)
{
    WriteLock lock(list_lock_);

    server_list_.erase(pipe_name);
    auto iter = session_list_.find(pipe_name);
    if (iter == session_list_.end())return err_code::e_fail;

    for (const auto& item : iter->second)
    {
        item->Close();
    }
}

smart_result _stdcall PipeMgr::ConnectToServer(const wchar_t* pipe_name, IPipeClientCallback* client_callback)
{
    if (!pipe_name || !client_callback)return err_code::e_invalidarg;

    auto pipe_handle = CreateFile(pipe_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);

    if (pipe_handle == INVALID_HANDLE_VALUE) 
    {
        pipe_handle = NULL;
        LOG(ERROR) << "ConnectToServer server:[" << pipe_name << "] failed, GLE:[" << errno << "]";
        return err_code::e_fail;
    }

    IPipeSessionPtr session;
    PipeSessionClient::CreateInstanceEx(session, client_callback, pipe_handle, pipe_name);

    auto pclient_session = dynamic_cast<PipeSessionClient*>(session.GetInterface());
    pclient_session->Start();

    client_callback->OnConnect(session);

    return err_code::s_ok;
}

void PipeMgr::BroadcastData(const wchar_t* pipe_name, void* data, int32_t len)
{
    ReadLock lock(list_lock_);
    auto iter = session_list_.find(pipe_name);

    if (iter == session_list_.end())return;

    for (const auto& client : iter->second)
    {
        client->Write(data, len);
    }
}

void PipeMgr::RemoveSession(const wchar_t* pipe_name, const IPipeSessionPtr& session)
{
    WriteLock lock(list_lock_);

    auto iter = session_list_.find(pipe_name);
    if (iter == session_list_.end())return ;

    auto session_iter = std::find(iter->second.begin(), iter->second.end(), session);

    if (session_iter != iter->second.end())
    {
        iter->second.erase(session_iter);
    }
}

void PipeMgr::AddSession(const wchar_t* pipe_name, const IPipeSessionPtr& session)
{
    WriteLock lock(list_lock_);
    session_list_[pipe_name].push_back(session);
}

smart_result PipeMgr::InternalCreateServer(const wchar_t* pipe_name, IPipeServerCallback* server_callback)
{
    auto pipe_handle=::CreateNamedPipe(pipe_name,
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 1024, 1024, NMPWAIT_USE_DEFAULT_WAIT, NULL
    );

    if (pipe_handle == nullptr || pipe_handle == INVALID_HANDLE_VALUE)
    {
        LOG(ERROR) << "CreateNamedPipe failed, GLE:[" << errno << "]";
        return err_code::e_fail;
    }

    return AsynWaitClientConnect(pipe_handle, pipe_name, server_callback);
}

smart_result PipeMgr::AsynWaitClientConnect(void* pipe_handle, const wchar_t* pipe_name, IPipeServerCallbackPtr server_callback)
{
    std::wstring pipename = pipe_name;

    InternalIOContext::instance().AddTask([this, pipe_handle, pipename, server_callback]
        {
            //连接完成后，系统会将OVERLAPPED的hEvent设置为已通知状态事件
            //这里创建一个事件赋值给hEvent，用来监控其改变
            auto hevent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (hevent == NULL || hevent == INVALID_HANDLE_VALUE)
            {
                CloseHandle(pipe_handle);
                return;
            }

            //2.等待客户端的连接
            OVERLAPPED ovlap;
            memset(&ovlap,0, sizeof(OVERLAPPED));//底层调用的memset
            ovlap.hEvent = hevent;
            bool connected{ false };

            if (!::ConnectNamedPipe(pipe_handle, &ovlap)) 
            {
                auto ret = ::GetLastError();
                if (ERROR_PIPE_CONNECTED == ret)
                {
                    connected = true;
                }

                //标准判断操作
                if (ERROR_PIPE_CONNECTED != ret && ERROR_IO_PENDING != ret)
                {
                    LOG(ERROR) << "ConnectNamedPipe failed:[" << ret << "]";
                    CloseHandle(pipe_handle);
                    return;
                }
            }

            if (!connected)
            {
                auto wait_res = WaitForSingleObject(hevent, 50);

                if (wait_res == WAIT_FAILED)
                {
                    CloseHandle(pipe_handle);
                    CloseHandle(hevent);
                    LOG(ERROR) << "WaitForSingleObject failed:[" << wait_res << "]";
                    return;
                }

                if (wait_res == WAIT_TIMEOUT)
                {
                    AsynWaitClientConnect(pipe_handle, pipename.c_str(), server_callback);
                    return;
                }
            }

            IPipeSessionPtr session;
            PipeSessionServer::CreateInstanceEx(session, server_callback, pipe_handle, pipename.c_str());
            server_callback->OnConnect(session);

            auto pserver_session = dynamic_cast<PipeSessionServer*>(session.GetInterface());
            pserver_session->Start();

            AddSession(pipename.c_str(), session);

            InternalCreateServer(pipename.c_str(), server_callback);

        });

    return err_code::s_ok;
}
