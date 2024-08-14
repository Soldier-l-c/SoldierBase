#pragma once
#include "InternalLogger.h"

class Logger :public ILogger, public Base::CObjectImpl<Logger>
{
public:

    BEGIN_OBJECT_MAP(Logger)
        OBJECT_INTERFACE(ILogger)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

public:
    virtual void __stdcall Init(const wchar_t* log_path);
   
    virtual void __stdcall Write(int32_t level, const wchar_t* stream) override;
  
    virtual void __stdcall SetProperty(logger::LoggerProperty type, int32_t value)override;
  
private:
    bool InitLogger(const wchar_t* log_path);

private:
    logger::LOG_LEVEL level_{ logger::LOG_LEVEL::LEVEL_INFO };
    bool log_thread_{ true };
    bool log_time_{ true };
    bool init_{ false };

#ifdef USE_INTERNAL_LOG
    std::shared_ptr<InternalBaseLogger> logger_;
#endif

};