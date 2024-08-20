#include "pch.h"
#include "CrashMgr.h"

void CrashMgr::Init()
{
	if (init_)return;
#ifdef WIN32
	::SetUnhandledExceptionFilter(UnHandledExceptionFilter);
	init_ = true;
#endif
}

void CrashMgr::UnInit()
{

}

#ifdef WIN32
void CrashMgr::HandleException(PEXCEPTION_POINTERS pExInfo)
{
	if (!InitDumpWriteFunc())return;
	if (!InitDumpFileDir())return;
	RemoveOldDumpFiles();

#ifdef WIN32
	WriteDumpTofile(pExInfo, GetDumpFileFullPath(true), MINIDUMP_TYPE::MiniDumpWithFullMemory);
#endif
}

LONG __stdcall CrashMgr::UnHandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
	CrashMgr::instance().HandleException(pExInfo);
	::TerminateProcess(::GetCurrentProcess(), -1);
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL CrashMgr::MiniDumpCallback(PVOID, const PMINIDUMP_CALLBACK_INPUT CallbackInput, PMINIDUMP_CALLBACK_OUTPUT)
{
	return TRUE;
}

bool CrashMgr::WriteDumpTofile(PEXCEPTION_POINTERS pExInfo, const std::wstring& file_path, MINIDUMP_TYPE type)
{
	auto hfile = ::CreateFile(file_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!hfile || hfile == INVALID_HANDLE_VALUE) return false;

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = ::GetCurrentThreadId();
	eInfo.ExceptionPointers = pExInfo;
	eInfo.ClientPointers = FALSE;

	MINIDUMP_CALLBACK_INFORMATION cbMiniDump;
	cbMiniDump.CallbackRoutine = CrashMgr::MiniDumpCallback;
	cbMiniDump.CallbackParam = 0;

	BOOL bRes = mini_dump_writer_(::GetCurrentProcess(),
		::GetCurrentProcessId(),
		hfile,
		type,
		&eInfo,
		NULL,
		&cbMiniDump);

	::CloseHandle(hfile);

	return true;
}
#endif

bool CrashMgr::InitDumpFileDir()
{
	auto cur_exe_file_name = helper::path::get_cur_exe_file_name();

	dump_file_dir_ = helper::path::get_cur_full_path_combine((std::wstring(L"dump\\") + cur_exe_file_name + L"\\").c_str());

	std::filesystem::path path(dump_file_dir_);
	auto parent = path.parent_path();
	std::error_code ec;
	if (!std::filesystem::exists(parent, ec))
	{
		std::filesystem::create_directories(parent, ec);
	}

	auto res = std::filesystem::exists(parent, ec) && std::filesystem::is_directory(parent, ec);

	return res;
}

void CrashMgr::RemoveOldDumpFiles()
{
	std::error_code ec;
	std::wstring dump_ext(L".dmp");

	std::filesystem::directory_iterator end;
	decltype(end) iter(dump_file_dir_, ec);

	for (; iter != end && !ec; iter.increment(ec)) 
	{
		auto ext = iter->path().extension().wstring();

		if (helper::String::is_equal_no_case(ext, dump_ext) == false) continue;

		std::filesystem::remove(*iter, ec);
	}
}

std::wstring CrashMgr::GetDumpFileFullPath(bool full)
{
	std::error_code err;

	auto t = helper::time::get_timestamp_ins();

	std::wstringstream ss;
	ss << dump_file_dir_ << helper::path::get_cur_exe_file_name();
	ss << std::put_time(std::localtime(&t), L"_%Y-%m-%d-%H-%M-%S_");
	ss << (full ? L"full.dmp" : L".dmp");

	return ss.str();
}

bool CrashMgr::InitDumpWriteFunc()
{
#ifdef WIN32
	auto hdll = ::LoadLibrary(L"dbghelp.dll");
	if (!hdll || hdll == INVALID_HANDLE_VALUE)return false;

	mini_dump_writer_ = (decltype(mini_dump_writer_))::GetProcAddress(hdll, "MiniDumpWriteDump");

	return nullptr != mini_dump_writer_;
#else
	return false;
#endif // WIN32

}
