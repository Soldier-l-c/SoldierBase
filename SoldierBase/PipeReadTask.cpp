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
	DWORD lasterr{ 0 };;
	DWORD data_len{ len };
	BOOL  read_res{ false };
	auto res{ false };

	do 
	{
		read_res = ::ReadFile(handle_, data, data_len, &data_len, &apped_);
		if (read_res)
		{
			LOG(INFO) << "ReadFile1 size:[" << len << "] data len:[" << data_len << "]";
			res = true;
			break;
		}
		else if (data_len > 0)
		{
			LOG(INFO) << "ReadFile2 size:[" << len << "] data len:[" << data_len << "]";
		}

		lasterr = ::GetLastError();
		if (ERROR_IO_PENDING == lasterr)
		{
			len = 0;
			res = true;
			break;
		}

	} while (false);
	
	/*
	do 
	{
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

		data_len = len;
		if (!GetOverlappedResult(handle_, &apped_, &data_len, FALSE)) 
		{
			lasterr = ::GetLastError();
			if (ERROR_IO_INCOMPLETE == lasterr)
			{
				len = 0;
				return true;
			}
			break;
		}

		read_res = ::ReadFile(handle_, data, data_len, &data_len, &apped_);
		
		if (read_res)
		{
			LOG(INFO) << "ReadFile2 size:[" << len << "] data len:[" << data_len << "]";
			return true;
		}

		res = (data_len == len);

		pending_ = false;

	} while (false);*/

	if (!res && ERROR_IO_PENDING!= lasterr)
	{
		callback_->OnError(lasterr);
		helper::time::sleep(50);
		return false;
	}

	helper::time::sleep(50);

	return res;
}
