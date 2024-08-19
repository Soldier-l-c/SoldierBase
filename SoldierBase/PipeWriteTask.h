#pragma once
#include "PipeContextTaskBase.h"
class PipeWriteTask :
	public PipeContextTaskBase,public std::enable_shared_from_this<PipeWriteTask>
{
public:
	PipeWriteTask(const stream_handle_ptr& pipe_handle, PipeContextCallbcak* callback, void* data, uint32_t len);

protected:
	virtual bool InternalRun();

	virtual void InternaleClose();

private:
	bool InitDataBuffer(void* data, uint32_t len);

private:
	NsPipeData::DataBufferPtr data_buffer_;
};
