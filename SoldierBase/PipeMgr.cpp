#include "pch.h"
#include "PipeMgr.h"

smart_result _stdcall PipeMgr::CreatePipeServer(const wchar_t* pip_name, IPipeServerCallback* server_callback)
{
    return smart_result();
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
    return smart_result();
}

smart_result _stdcall PipeMgr::ConnectToServer(const wchar_t* pipe_name, IPipeClientCallback* client_callback)
{
    return smart_result();
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
