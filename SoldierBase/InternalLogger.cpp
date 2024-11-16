#include "pch.h"
#include "InternalLogger.h"

void InternalBaseLogger::Init(const wchar_t* log_path)
{
	log_name_ = log_path;
}

bool InternalBaseLogger::Run()
{
	decltype(asyn_log_list_) temp_list;
	{
		WriteLock lock(asyn_log_lock_);
		if (asyn_log_list_.empty())
			return false;

		temp_list = std::move(asyn_log_list_);
		asyn_log_list_.clear();
	}

	for (const auto& l : temp_list)
	{
		InternalWrite(l);
	}

	return false;
}

void InternalBaseLogger::Write(int32_t level, const wchar_t* buffer)
{
	if (level > level_  || !buffer)return;

	auto real_buffer = GetRealWriteBuffer(level, buffer);

	if (asyn_log_)
	{
		WriteLock lock(asyn_log_lock_);
		asyn_log_list_.push_back(std::move(real_buffer));
		InternalIOContext::instance().AddTask([self = shared_from_this()] 
			{
				self->Run();
			});

		return;
	}

	InternalWrite(real_buffer);
}

void InternalBaseLogger::SetProperty(logger::LoggerProperty type, int32_t value)
{
	switch (type)
	{
	case logger::LoggerProperty::Level:
		level_ = (decltype(level_))value;
		break;
	case logger::LoggerProperty::LogThread:
		log_thread_ = value > 0;
		break;
	case logger::LoggerProperty::LogTime:
		log_time_ = value > 0;
		break;
	case logger::LoggerProperty::AsynLog:
		asyn_log_ = value > 0;
		break;
	}
}

InternalBaseLogger::~InternalBaseLogger()
{
	if (!init_)return;
	if (file_handle_)
	{
#ifdef WIN32
		CloseHandle(file_handle_);
#endif
	}
}

void InternalBaseLogger::InternalWrite(const std::wstring& buffer)
{
	std::lock_guard<std::mutex>lock(write_lock_);

	bool log_date = ((write_count_++ % count_to_log_date_) == 0);
	write_count_ = write_count_ % count_to_log_date_;

	if (log_date)
	{
		WriteToFile(GetLogDateBuffer() + L"\r\n");
	}

	WriteToFile(buffer);
}

std::wstring InternalBaseLogger::GetLogPrefix(int32_t level)
{
	std::wstringstream stream;

	if (log_time_)
	{
		stream << GetLogTimeBuffer();
		stream.width(8);
	}

	if (log_thread_)
	{
		stream << std::this_thread::get_id();
		stream.width(4);
		stream << "";
	}

	stream.width(4);
	stream << level;

	stream.width(4);
	stream << "";

	return stream.str();
}

std::wstring InternalBaseLogger::GetRealWriteBuffer(int32_t level, const wchar_t* buffer)
{
	auto real_write = GetLogPrefix(level);

	real_write += buffer;

	if (real_write.length() > 2 && real_write[real_write.length() - 1] != L'\n')
	{
		real_write.append(L"\r\n");
	}

	return real_write;
}

std::wstring InternalBaseLogger::GetLogDateBuffer()
{
	time_t cur_s_time = helper::time::get_timestamp_ins();

	tm t_tm;
	localtime_s(&t_tm, &cur_s_time);

	wchar_t buffer[64] = { 0 };
	_snwprintf(buffer, 64, L"%04d-%02d-%02d",
		t_tm.tm_year + 1900,
		t_tm.tm_mon + 1,
		t_tm.tm_mday);

	return (buffer);
}

std::wstring InternalBaseLogger::GetLogTimeBuffer()
{
	auto cur_ms_time = helper::time::get_timestamp_inms();
	time_t cur_s_time = cur_ms_time / 1000;
	tm t_tm;
	localtime_s(&t_tm, &cur_s_time);

	wchar_t buffer[64] = { 0 };
	_snwprintf(buffer, 64, L"%02d:%02d:%02d.%03d",
		t_tm.tm_hour,
		t_tm.tm_min,
		t_tm.tm_sec,
		(int32_t)(cur_ms_time % 1000));

	return buffer;
}

void InternalBaseLogger::WriteToFile(const std::wstring& buffer)
{
	InitFileHandle();

	if (nullptr == file_handle_ || file_handle_ == INVALID_HANDLE_VALUE)return;

	auto write_count = (unsigned long)buffer.length() * sizeof(wchar_t);
#ifdef WIN32
	ReSetFilePointer();
	WriteFile(file_handle_, buffer.c_str(), write_count, &write_count, nullptr);
#endif
}

bool InternalBaseLogger::InitLogDir(const wchar_t* log_path)
{
	log_dir_ = helper::path::get_cur_full_path_combine((std::wstring(L"log\\") + log_path +L"\\").c_str());

	std::filesystem::path path(log_dir_);
	auto parent = path.parent_path();
	std::error_code ec;
	if (!std::filesystem::exists(parent, ec))
	{
		std::filesystem::create_directories(parent, ec);
	}

	auto res = std::filesystem::exists(parent, ec) && std::filesystem::is_directory(parent, ec);;

	return res;
}

std::wstring InternalBaseLogger::GetLogFileName(const wchar_t* log_path)
{
	std::wstringstream stream;

	stream << helper::path::get_cur_exe_file_name();
	stream << ("_") << log_path << "_" << GetLogDateBuffer();

#ifdef WIN32
	stream <<"_"  << GetCurrentProcessId();
#endif

	stream << ".log";

	return stream.str();
}

void InternalBaseLogger::ReSetFilePointer()
{
#ifdef WIN32
	DWORD size_high = 0;
	DWORD size_low = 0;

	size_low = GetFileSize(file_handle_, &size_high);
	if (size_high != 0 || size_low > max_file_size_)
	{
		SetFilePointer(file_handle_, 0, NULL, FILE_BEGIN);
		SetEndOfFile(file_handle_);
		size_low = 0;
	}

	if (size_low == 0)
	{
		DWORD tag = 0xFEFF;
		DWORD nHave = 0;
		WriteFile(file_handle_, &tag, 2, &nHave, NULL);
	}

	SetFilePointer(file_handle_, 0, 0, FILE_END);
#endif
}

void InternalBaseLogger::InitFileHandle()
{
	if (init_)return;

	InitLogDir(log_name_.c_str());
	InitLogFileHandle(GetLogFileName(log_name_.c_str()));

	init_ = true;
}

bool InternalBaseLogger::InitLogFileHandle(const std::wstring& log_file_name)
{
#ifdef WIN32
	auto logfile_path = log_dir_ + L"\\" + log_file_name;
	
	file_handle_ = CreateFile(logfile_path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle_ == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	ReSetFilePointer();
	
	return true;
#else
	return false;
#endif
	return false;
}
