#pragma once
#include <base/ITimer.h>
class Timer :
    public ITimer,
    public Base::CObjectImpl<Timer>
{
    BEGIN_SINGLETON_OBJECT_MAP(Timer)
        OBJECT_INTERFACE(ISmartBase)
        OBJECT_INTERFACE(ITimer)
    END_OBJECT_MAP();

    virtual smart_result __stdcall AddTimer(uint32_t time_id, BASE_TIMER_CALLBACK call_back, void* context, uint32_t interval, int32_t call_count);

};

