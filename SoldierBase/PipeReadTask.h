#pragma once
#include "PipeContextTaskBase.h"
class PipeContextReadTask : public PipeContextTaskBase, public std::enable_shared_from_this<PipeContextReadTask>
{
public:
	PipeContextReadTask(const stream_handle_ptr& pipe_handle, PipeContextCallbcak* callback);
	~PipeContextReadTask();

protected:
	virtual bool InternalRun();

	virtual void InternaleClose();

	bool InternalRead();

	bool HeaderValid();

	bool ReadData();

	bool ReadHeader();

private:
	NsPipeData::DataHeader header_;
	helper::SmartAllocator<uint8_t> alloc_;
};

using AsynReaderPtr = std::shared_ptr<PipeContextReadTask>;