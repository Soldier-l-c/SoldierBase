#include "pch.h"
#include "Logger.h"

void __stdcall Logger::Init(const wchar_t* log_path)
{
    InitLogger(log_path);
    init_ = true;
}

void __stdcall Logger::Write(int32_t level, const wchar_t* stream)
{
#ifdef USE_INTERNAL_LOG
    return  logger_->Write(level, stream);
#endif

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
#ifdef USE_INTERNAL_LOG
    return  logger_->SetProperty(type, value);
#endif

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
}

bool Logger::InitLogger(const wchar_t* log_path)
{
#ifdef USE_INTERNAL_LOG 
    logger_ = std::make_shared<InternalBaseLogger>();
    logger_->Init(log_path);
#else
    if (google::IsGoogleLoggingInitialized())return true;
    static auto log_file_name = helper::String::utf16_to_utf8(log_path);
    google::InitGoogleLogging(log_file_name.c_str());
    FLAGS_log_dir = helper::String::utf16_to_utf8(helper::path::get_cur_full_path().c_str());
#endif
    return true;
}