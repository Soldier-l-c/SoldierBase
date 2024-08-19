#include "pch.h"
#include "PipeContextTaskBase.h"

PipeContextTaskBase::PipeContextTaskBase(const stream_handle_ptr& pipe_handle, PipeContextCallbcak* callback):
	handle_(pipe_handle),closed_(false),callback_(callback)
{
	
}

bool PipeContextTaskBase::Run()
{
	ReadLock lock(handle_lock_);

	if (!handle_ || INVALID_HANDLE_VALUE == handle_->native_handle() || closed_)
		return false;

	return InternalRun();
}

void PipeContextTaskBase::OnClose()
{
	WriteLock lock(handle_lock_);

	if (!handle_ || INVALID_HANDLE_VALUE == handle_->native_handle() || closed_)
		return;

	InternaleClose();

	closed_ = true;
}

bool PipeContextTaskBase::IsValid()
{
	return !closed_ && nullptr != handle_ && handle_->is_open();
}
