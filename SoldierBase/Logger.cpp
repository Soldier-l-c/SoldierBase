#include "pch.h"
#include "Logger.h"

void InitLogger(const wchar_t* log_path)
{
#ifndef USEGLOG 
    if (g_logger == nullptr)
    {
        g_logger = std::make_shared<console_logger::InternalLogger>();
        g_logger->SetProperty(logger::LoggerProperty::Level, (int32_t)logger::LEVEL_INFO);
    }
#else
    if (google::IsGoogleLoggingInitialized())return;
    static auto log_file_name = helper::String::utf16_to_utf8(log_path);
    google::InitGoogleLogging(log_file_name.c_str());
    FLAGS_log_dir = helper::String::utf16_to_utf8(helper::path::get_cur_full_path().c_str());
#endif
}

void __stdcall Logger::Init(const wchar_t* log_path)
{
    InitLogger(log_path);
    init_ = true;
}

void __stdcall Logger::Write(int32_t level, const wchar_t* stream)
{
    if (level > level_)return;

    switch (level)
    {
    case logger::LEVEL_WARNING:
        LOG(WARNING) << helper::String::utf16_to_utf8(stream);
        break;
    case logger::LEVEL_INFO:
        LOG(INFO) << helper::String::utf16_to_utf8(stream);
        break;
    case logger::LEVEL_ERROR:
        LOG(ERROR) << helper::String::utf16_to_utf8(stream);
        break;
    case logger::LEVEL_FATAL:
        LOG(FATAL) << helper::String::utf16_to_utf8(stream);
        break;
    }
}
void __stdcall Logger::SetProperty(logger::LoggerProperty type, int32_t value)
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
    }
};