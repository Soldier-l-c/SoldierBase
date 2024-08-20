#include "pch.h"
#include "PipeServer.h"
#include "PipeMgr.h"

PipeServer::PipeServer(const std::wstring& pipe_name):pipe_name_(pipe_name)
{
}

void _stdcall PipeServer::BroadcastData(void* data, int32_t len)
{
	PipeMgr::instance().BroadcastData(pipe_name_.c_str(), data, len);
}
