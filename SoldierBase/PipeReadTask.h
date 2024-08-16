#pragma once
#include "PipeContextTaskBase.h"
class PipeContextReadTask : public PipeContextTaskBase
{
public:
	PipeContextReadTask(void* handle, PipeContextCallbcak* callback);
	~PipeContextReadTask();

protected:
	virtual bool InternalRun();

	virtual void InternaleClose();

	bool InternalRead();

	bool HeaderValid();

	bool ReadData();

	bool ReadData(void* data, uint32_t& len);

private:
	NsPipeData::DataHeader header_;
	OVERLAPPED apped_;

};