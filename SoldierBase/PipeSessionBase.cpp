#include "pch.h"
#include "PipeSessionBase.h"
#include "PipeWriteTask.h"
#include "InternalIOContext.h"
#include "PipeMgr.h"
#include "PipeReadTask.h"

PipeSessionBase::PipeSessionBase(void* pipe_handle, const std::wstring& pipe_name):
	pipe_handle_(pipe_handle),pipe_name_(pipe_name)
{
	
}

void PipeSessionBase::Start()
{
	AsynReadData();
}

smart_result _stdcall PipeSessionBase::Write(void* data, int32_t len)
{
	AsynWrite(data, len);
	return err_code::s_ok;
}

smart_result _stdcall PipeSessionBase::Close()
{
	if (closed_)return err_code::error_success;
	closed_ = true;
	IniternalClose(-1);
	return err_code::s_ok;
}

void PipeSessionBase::OnError(uint32_t err_code)
{
	if (closed_)return;
	closed_ = true;
	
	IniternalClose(err_code);
}

bool PipeSessionBase::AsynWrite(void* data, int32_t len)
{
	if (closed_)return false;

	IPipeSessionPtr session(this);
	auto write_task = std::make_shared<PipeWriteTask>(pipe_handle_, this, data, len);

	InternalIOContext::instance().AddTask([session,this, write_task](void)
		{
			if (closed_)return;

			write_task->Run();
		});
	return true;
}

bool PipeSessionBase::SynWrite(void* data, int32_t len)
{
	if (closed_)return false;

	auto write_task = std::make_shared<PipeWriteTask>(pipe_handle_, this, data, len);
	return write_task->Run();
}

bool PipeSessionBase::AsynReadData()
{
	if (closed_)return false;

	IPipeSessionPtr session(this);
	InternalIOContext::instance().AddTask([session, this](void)
		{
			if (closed_)return;
			auto write_task = std::make_shared<PipeContextReadTask>(pipe_handle_, this);
			if (write_task->Run())
			{
				AsynReadData();
			}
		});

	return true;
}
