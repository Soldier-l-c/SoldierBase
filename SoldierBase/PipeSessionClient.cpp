#include "pch.h"
#include "PipeSessionClient.h"

PipeSessionClient::PipeSessionClient(IPipeClientCallback* callback, const stream_handle_ptr& pipe_handle, const std::wstring& pipe_name):
	callback_(callback), PipeSessionBase(pipe_handle, pipe_name)
{
}

void PipeSessionClient::OnDataRecived(uint8_t* data, int32_t len)
{
	callback_->OnReciveData(data, len);
}

void PipeSessionClient::IniternalClose(uint32_t err_code)
{
	pipe_handle_->close();
	callback_->OnDisconnect(err_code);
	callback_ = static_cast<IPipeClientCallback*>(nullptr);
}
