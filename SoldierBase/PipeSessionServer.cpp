#include "pch.h"
#include "PipeSessionServer.h"
#include "PipeMgr.h"

PipeSessionServer::PipeSessionServer(IPipeServerCallback* callback, void* pipe_handle, const std::wstring& pipe_name):
	callback_(callback),PipeSessionBase(pipe_handle, pipe_name)
{
}

void PipeSessionServer::OnDataRecived(uint8_t* data, int32_t len)
{
	callback_->OnReciveData(this, data, len);
}

void PipeSessionServer::OnError(uint32_t err_code)
{
	PipeSessionBase::OnError(err_code);
	PipeMgr::instance().RemoveSession(pipe_name_.c_str(), this);
}

void PipeSessionServer::IniternalClose(uint32_t err_code)
{
	::DisconnectNamedPipe(pipe_handle_);
	pipe_handle_ = nullptr;
	callback_->OnDisconnect(this, err_code);
	callback_ = nullptr;
}
