#pragma once
#include "PipeContextTaskBase.h"
class PipeWriteTask :
    public PipeContextTaskBase
{
public:
	PipeWriteTask(void* handle, PipeContextCallbcak* callback, void* data, uint32_t len);

protected:
	virtual bool InternalRun();

	virtual void InternaleClose();

private:
	bool InitDataBuffer(void* data, uint32_t len);

private:
	NsPipeData::DataBufferPtr data_buffer_;
};
