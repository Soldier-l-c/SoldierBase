#include "pch.h"
#include "PipeSessionBase.h"
#include "PipeWriteTask.h"
#include "InternalIOContext.h"

smart_result _stdcall PipeSessionBase::Write(void* data, int32_t len)
{
	
}

smart_result _stdcall PipeSessionBase::Close()
{
	return smart_result();
}

void PipeSessionBase::OnError(uint32_t err_code)
{

}

void PipeSessionBase::OnDataRecived(uint8_t* data, int32_t len)
{

}

bool PipeSessionBase::AsynWrite(void* data, int32_t len)
{
	auto write_task = std::make_shared<PipeWriteTask>(pipe_handle_, this, data, len);
	InternalIOContext::instance().AddRunOnceTask(write_task);

	return true;
}

bool PipeSessionBase::SynWrite(void* data, int32_t len)
{
	auto write_task = std::make_shared<PipeWriteTask>(pipe_handle_, this, data, len);
	return write_task->Run();
}

void PipeSessionBase::IniternalClose(uint32_t err_code)
{
}
