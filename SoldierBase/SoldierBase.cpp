#include "pch.h"
#include "SoldierBase.h"
#include "CrashMgr.h"
#include "InternalIOContext.h"

smart_result __stdcall SoldierBase::Initialize()
{
    if (init_)return err_code::error_success;
    init_ = true;

    CrashMgr::instance().Init();

    InternalIOContext::instance().Init();

    return err_code::error_success;
}

smart_result __stdcall SoldierBase::UnInitialize()
{
    if (!init_)return err_code::error_success;
    init_ = false;

    CrashMgr::instance().UnInit();
    InternalIOContext::instance().UnInit();

    return err_code::error_success;
}
