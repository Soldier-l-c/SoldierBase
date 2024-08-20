#pragma once
#include "PipeMgr.h"
#include "PipeContextTaskBase.h"

class PipeSessionBase : 
	public IPipeSession,
	public PipeContextCallbcak
{
public:

	PipeSessionBase(const stream_handle_ptr& pipe_handle, const std::wstring& pipe_name);

	~PipeSessionBase();

	virtual void Start();

	virtual smart_result _stdcall Write(void* data, int32_t len);

	virtual smart_result _stdcall Close();

	virtual void OnError(uint32_t err_code);

	bool IsValid();

protected:

	bool AsynWrite(void* data, int32_t len);

	virtual void IniternalClose(uint32_t err_code) = 0;

	bool AsynReadData();

protected:
	stream_handle_ptr pipe_handle_{ nullptr };
	std::wstring pipe_name_;
	std::atomic_bool closed_{ false };
};

