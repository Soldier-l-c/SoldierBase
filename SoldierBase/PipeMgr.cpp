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

    return err_code::s_ok;
}

smart_result _stdcall PipeMgr::ConnectToServer(const wchar_t* pipe_name, IPipeClientCallback* client_callback)
{
    if (!pipe_name || !client_callback)return err_code::e_invalidarg;

    auto pipe_handle = CreateFile(GetRealPipeName(pipe_name).c_str(),
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

    auto& io_context = InternalIOContext::instance().IOContext();
    auto handle = std::make_shared<boost_stream_handle>(io_context, pipe_handle);
    IPipeSessionPtr session;
    PipeSessionClient::CreateInstanceEx(session, client_callback, handle, pipe_name);

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

    auto session_iter = std::find_if(iter->second.begin(), iter->second.end(),
        [session](const IPipeSessionPtr& s) {return (s == session);});

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
    auto pipe_handle=::CreateNamedPipe(GetRealPipeName(pipe_name).c_str(),
        PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES, 4096, 4096, NMPWAIT_USE_DEFAULT_WAIT, NULL
    );

    if (pipe_handle == nullptr || pipe_handle == INVALID_HANDLE_VALUE)
    {
        LOG(ERROR) << "CreateNamedPipe failed, GLE:[" << errno << "]";
        return err_code::e_fail;
    }

    auto& io_context = InternalIOContext::instance().IOContext();
    auto handle = std::make_shared<boost_stream_handle>(io_context, pipe_handle);

    return AsynWaitClientConnect(handle, pipe_name, server_callback);
}

smart_result PipeMgr::AsynWaitClientConnect(stream_handle_ptr& handle, const wchar_t* pipe_name, IPipeServerCallbackPtr server_callback)
{
    std::wstring pipename = pipe_name;

    auto create_session_func = [this, server_callback, pipename, handle](boost::system::error_code const& error, size_t bytesTransferred)
    {
        IPipeSessionPtr session;
        PipeSessionServer::CreateInstanceEx(session, server_callback, handle, pipename.c_str());
        server_callback->OnConnect(session);

        auto pserver_session = dynamic_cast<PipeSessionServer*>(session.GetInterface());
        pserver_session->Start();

        AddSession(pipename.c_str(), session);

        //一个链接已经建立成功，再异步等待下一个链接
        InternalCreateServer(pipename.c_str(), server_callback);
    };

    InternalIOContext::instance().AddTask([this, handle, create_session_func]
        {
            auto& io_context = InternalIOContext::instance().IOContext();

            boost::asio::windows::overlapped_ptr overlapped(io_context, std::move(create_session_func));
            ::ConnectNamedPipe(handle->native_handle(), overlapped.get());
           
            auto last_error = ::GetLastError();

            if (last_error != ERROR_IO_PENDING)
            {
                boost::system::error_code error(last_error, boost::asio::error::get_system_category());
                overlapped.complete(error, 0);
            }
            else
            {
                overlapped.release();
            }
        });

    return err_code::s_ok;
}

std::wstring PipeMgr::GetRealPipeName(const wchar_t* pipe_name)
{
    static const wchar_t* PipePreFix = L"\\\\.\\pipe\\";
    return std::wstring(PipePreFix) + pipe_name;
}
