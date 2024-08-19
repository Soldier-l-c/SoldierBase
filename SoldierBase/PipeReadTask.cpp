#include "pch.h"
#include "PipeReadTask.h"

PipeContextReadTask::PipeContextReadTask(const stream_handle_ptr& pipe_handle, PipeContextCallbcak* callback):PipeContextTaskBase(pipe_handle, callback)
{
}

PipeContextReadTask::~PipeContextReadTask()
{
}

bool PipeContextReadTask::InternalRun()
{
	return InternalRead();
}

void PipeContextReadTask::InternaleClose()
{

}

bool PipeContextReadTask::InternalRead()
{
	ReadHeader();
	return true;
}

bool PipeContextReadTask::HeaderValid()
{
	return header_.data_length > 0 && header_.data_length <= NsPipeData::MaxPipeDataLength;
}

bool PipeContextReadTask::ReadData()
{
	if (!IsValid())
	{
		callback_->OnError(-3);
		return false;
	}

	if (!HeaderValid())
	{
		callback_->OnError(-1);
		return false;
	}

	auto buffer = alloc_.Allocetor(header_.data_length);
	if (buffer == nullptr)
	{
		callback_->OnError(-2);
		return false;
	}

	//self 防止被析构
	auto task = [this, self = shared_from_this(), buffer](const boost::system::error_code& error, std::size_t cb)
	{
		if (error.failed() || cb != (header_.data_length))
		{
			self->callback_->OnError(error.value());
			return;
		}

		self->callback_->OnDataRecived(buffer, header_.data_length);

		self->alloc_.Deallocate();

		//异步读取数据头
		ReadHeader();
	};

	handle_->async_read_some(
		boost::asio::buffer(buffer, header_.data_length), std::move(task));

	return true;
}

bool PipeContextReadTask::ReadHeader()
{
	if(!IsValid())
	{
		callback_->OnError(-3);
		return false;
	}

	memset(&header_, 0, sizeof(header_));
	uint32_t len{ sizeof(header_) };

	auto task = [this, self = shared_from_this()](const boost::system::error_code& error, std::size_t cb)
	{
		if (error.failed() || cb != sizeof(header_))
		{
			self->callback_->OnError(error.value());
			return;
		}

		//异步读取数据体
		self->ReadData();
	};

	handle_->async_read_some(
		boost::asio::buffer(&header_, sizeof(header_)), std::move(task));

	return true;
}

