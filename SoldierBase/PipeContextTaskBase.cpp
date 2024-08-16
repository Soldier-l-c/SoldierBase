#include "pch.h"
#include "PipeContextTaskBase.h"

PipeContextTaskBase::PipeContextTaskBase(void* handle, PipeContextCallbcak* callback):
	handle_(handle),closed_(false),callback_(callback)
{
	
}

bool PipeContextTaskBase::Run()
{
	ReadLock lock(handle_lock_);

	if (!handle_ || INVALID_HANDLE_VALUE == handle_ || closed_)
		return false;

	return InternalRun();
}

void PipeContextTaskBase::OnClose()
{
	WriteLock lock(handle_lock_);

	if (!handle_ || INVALID_HANDLE_VALUE == handle_ || closed_)
		return;

	InternaleClose();

	closed_ = true;
	handle_ = INVALID_HANDLE_VALUE;
}
