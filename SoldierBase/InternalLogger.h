#pragma once
#include "InternalIOContext.h"

class InternalBaseLogger : public IOTask, public std::enable_shared_from_this<InternalBaseLogger>
{
public:
	~InternalBaseLogger();

	void Init(const wchar_t* log_path);

	virtual void Run() override;

	void Write(int32_t level, const wchar_t* buffer);

	void SetProperty(logger::LoggerProperty type, int32_t value);

private:
	void InternalWrite(const std::wstring& buffer);

	std::wstring GetLogPrefix(int32_t level);

	std::wstring GetRealWriteBuffer(int32_t level, const wchar_t* buffer);
	
	std::wstring GetLogDateBuffer();

	std::wstring  GetLogTimeBuffer();

	void WriteToFile(const std::wstring& buffer);

	bool InitLogDir(const wchar_t* log_path);

	bool InitLogFileHandle(const std::wstring& log_file_name);

	std::wstring GetLogFileName(const wchar_t* log_path);

	void ReSetFilePointer();

private:
	logger::LOG_LEVEL level_{ logger::LOG_LEVEL::LEVEL_INFO };
	bool log_thread_{ true };
	bool log_time_{ true };
	bool init_{ false };
	bool asyn_log_{ false };

	std::mutex write_lock_;
	std::shared_mutex asyn_log_lock_;
	std::vector<std::wstring> asyn_log_list_;

	void* file_handle_{ nullptr };

	int32_t write_count_{ 0 };
	const int32_t count_to_log_date_{ 20 };//Ĭ��ÿ20����־��ӡһ������
	const int32_t max_file_size_{ 30 * 1024 * 1024 };

	std::wstring log_dir_;
};
