#pragma once
#include "PipeContextTaskBase.h"

class PipeSessionBase : 
	public IPipeSession,
	public PipeContextCallbcak
{
public:
	virtual smart_result _stdcall Write(void* data, int32_t len);

	virtual smart_result _stdcall Close();

	virtual void OnError(uint32_t err_code);

	virtual void OnDataRecived(uint8_t* data, int32_t len);

protected:
	bool AsynWrite(void* data, int32_t len);

	bool SynWrite(void* data, int32_t len);

	virtual void IniternalClose(uint32_t err_code);

protected:
	void* pipe_handle_{ nullptr };
	std::wstring pip_name;
};

