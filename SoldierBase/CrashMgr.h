#pragma once

#include <Imagehlp.h>
class CrashMgr
{
	SINGLE_INSTANCE(CrashMgr);

public:
	void Init();
	void UnInit();


#ifdef WIN32
	void HandleException(PEXCEPTION_POINTERS pExInfo);

	static LONG WINAPI UnHandledExceptionFilter(PEXCEPTION_POINTERS pExInfo);

	static BOOL CALLBACK MiniDumpCallback(PVOID, CONST PMINIDUMP_CALLBACK_INPUT CallbackInput, PMINIDUMP_CALLBACK_OUTPUT);
#endif

private:
	bool InitDumpFileDir();

	std::wstring GetDumpFileFullPath(bool full);

	bool InitDumpWriteFunc();

#ifdef WIN32
	bool WriteDumpTofile(PEXCEPTION_POINTERS pExInfo, const std::wstring& file_path, MINIDUMP_TYPE type);
#endif

private:
	bool init_{ false };

	decltype(&::MiniDumpWriteDump)   mini_dump_writer_{ nullptr };
	std::wstring dump_file_dir_;
};

