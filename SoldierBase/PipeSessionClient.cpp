#include "pch.h"
#include "PipeSessionClient.h"

PipeSessionClient::PipeSessionClient(IPipeClientCallback* callback):callback_(callback)
{

}

void PipeSessionClient::IniternalClose(uint32_t err_code)
{
	callback_->OnDisconnect(err_code);
	callback_ = nullptr;
}
