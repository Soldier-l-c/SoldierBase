#include "pch.h"
#include "PipeReadTask.h"

PipeContextReadTask::PipeContextReadTask(void* handle, PipeContextCallbcak* callback):PipeContextTaskBase(handle, callback)
{
	memset(&apped_, 0, sizeof(apped_));
	apped_.hEvent = ::CreateEvent(nullptr, TRUE, FALSE, 0);
}

PipeContextReadTask::~PipeContextReadTask()
{
	if (apped_.hEvent)
	{
		::CloseHandle(apped_.hEvent);
	}
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
	memset(&header_, 0, sizeof(header_));
	uint32_t len{ sizeof(header_) };

	if (!ReadData(&header_, len))
	{
		return false;
	}

	if (len == 0)
	{
		return true;
	}

	if (!HeaderValid())
	{
		callback_->OnError(-1);
		return false;
	}

	return ReadData();
}

bool PipeContextReadTask::HeaderValid()
{
	return header_.data_length > 0 && header_.data_length <= NsPipeData::MaxPipeDataLength;
}

bool PipeContextReadTask::ReadData()
{
	helper::SmartAllocator<uint8_t>alloc;
	auto buffer = alloc.Allocetor(header_.data_length);
	if (buffer == nullptr)return false;

	auto read_res = ReadData(buffer, header_.data_length);

	if (read_res)
	{
		callback_->OnDataRecived(buffer, header_.data_length);
	}

	return read_res;
}

bool PipeContextReadTask::ReadData(void* data, uint32_t& len)
{
	DWORD data_len{ len };
	::ReadFile(handle_, data, data_len, &data_len, &apped_);

	auto lasterr = ::GetLastError();
	auto res{ false };
	do 
	{
		if (lasterr != ERROR_IO_PENDING)
		{
			break;
		}

		const auto wait_res = ::WaitForSingleObject(apped_.hEvent, 50);

		if (wait_res == WAIT_TIMEOUT)
		{
			len = 0;
			return true;
		}

		if (wait_res == WAIT_FAILED)
		{
			lasterr = ::GetLastError();
			break;
		}

		if (!GetOverlappedResult(handle_, &apped_, &data_len, FALSE)) 
		{
			lasterr = ::GetLastError();
			break;
		}

		res = (data_len == len);

	} while (false);

	if (!res)
	{
		callback_->OnError(lasterr);
		return false;
	}

	return res;
}
