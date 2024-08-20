#include "pch.h"
#include "Timer.h"
#include "TimerMgr.h"

smart_result __stdcall Timer::AddTimer(uint32_t time_id, BASE_TIMER_CALLBACK call_back, void* context, uint32_t interval, int32_t call_count)
{
    if (call_back == nullptr || 0 == interval || 0 == call_count)
        return err_code::e_invalidarg;

    TimerMgr::instance().AddTimer(time_id, call_back, context, interval, call_count);
    return err_code::s_ok;
}
