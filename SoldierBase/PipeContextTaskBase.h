#pragma once
#include "InternalIOContext.h"

namespace NsPipeData
{
	static const uint32_t MaxPipeDataLength = 65536;

	using DataBuffer = std::vector<uint8_t>;
	using DataBufferPtr = std::unique_ptr<DataBuffer>;

	struct DataHeader
	{
		uint32_t data_length{ 0 };
		uint64_t reserved_data[4]{ 0 };
	};
	const uint32_t HeaderSize = sizeof(DataHeader);
}

struct PipeContextCallbcak
{
	virtual void OnError(uint32_t err_code) = 0;
	virtual void OnDataRecived(uint8_t* data, int32_t len) = 0;
};

class PipeContextTaskBase : public IOTask
{
public:
	PipeContextTaskBase(void* handle, PipeContextCallbcak* callback);

	bool Run() final;

	void OnClose();

protected:
	virtual bool InternalRun() = 0;

	virtual void InternaleClose() = 0;

protected:
	bool closed_{ false };
	void* handle_{ nullptr };
	std::shared_mutex handle_lock_;

	PipeContextCallbcak* callback_{ nullptr };
};