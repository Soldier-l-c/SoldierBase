#pragma once

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
    logger::LOG_LEVEL level_{ logger::LOG_LEVEL::LEVEL_INFO };
    bool log_thread_{ true };
    bool log_time_{ true };
    bool init_{ false };
};