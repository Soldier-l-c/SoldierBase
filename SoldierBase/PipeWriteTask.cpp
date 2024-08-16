#include "pch.h"
#include "PipeWriteTask.h"

PipeWriteTask::PipeWriteTask(void* handle, PipeContextCallbcak* callback, void* data, uint32_t len) :PipeContextTaskBase(handle, callback)
{
	InitDataBuffer(data, len);
}

bool PipeWriteTask::InternalRun()
{
	DWORD len{ 0 };
	::WriteFile(handle_, data_buffer_->data(), data_buffer_->size(), &len, nullptr);
	if (len != data_buffer_->size())
	{
		callback_->OnError(::GetLastError());
	}

	return false;
}

void PipeWriteTask::InternaleClose()
{

}

bool PipeWriteTask::InitDataBuffer(void* data, uint32_t len)
{
	if (!data || len == 0 || len > NsPipeData::MaxPipeDataLength)
		return false;

	data_buffer_ = std::make_unique<decltype(data_buffer_)::element_type>();

	auto real_len = len + NsPipeData::HeaderSize;

	data_buffer_->resize(real_len);
	auto buffer = data_buffer_->data();

	auto pheader = reinterpret_cast<NsPipeData::DataHeader*>(buffer);
	memset(pheader, 0, NsPipeData::HeaderSize);

	pheader->data_length = len;
	memcpy(buffer + NsPipeData::HeaderSize, data, len);

	return true;
}
