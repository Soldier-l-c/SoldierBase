#include "pch.h"
#include "PipeWriteTask.h"

PipeWriteTask::PipeWriteTask(const stream_handle_ptr& pipe_handle, PipeContextCallbcak* callback, void* data, uint32_t len) :PipeContextTaskBase(pipe_handle, callback)
{
	InitDataBuffer(data, len);
}

bool PipeWriteTask::InternalRun()
{
	if (!IsValid())
	{
		callback_->OnError(-3);
		return false;
	}

	auto* pdata = data_buffer_->data();
	auto ndata = data_buffer_->size();

	//self 防止被析构
	auto task = [this, self = shared_from_this(), safe_data = std::move(data_buffer_)](const boost::system::error_code& error, std::size_t cb)
	{
		if (error.failed() || cb != safe_data->size())
		{
			callback_->OnError(error.value());
			return;
		}
	};

	handle_->async_write_some(boost::asio::buffer(pdata, ndata), std::move(task));

	return true;
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
