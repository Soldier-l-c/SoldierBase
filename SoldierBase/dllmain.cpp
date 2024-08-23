// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "Logger.h"
#include "NetHelper.h"
#include "DataBundle.h"
#include "DataArray.h"
#include "Timer.h"
#include "SoldierBase.h"
#include "PipeMgr.h"

#ifdef WIN32
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#endif

class Demo :
    public Base::CObjectImpl<Demo>,
    public IDemo
{
public:
    Demo() { LOG(INFO) << "Demo"; };
    virtual ~Demo() { LOG(INFO) << "~Demo"; };

public:
    BEGIN_OBJECT_MAP(Demo)
        OBJECT_INTERFACE(IDemo)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

public:
    virtual const char* __stdcall Test()override { return "Demo test!"; };
};

void InitLogger()
{
#ifdef USE_INTERNAL_LOG
    static std::once_flag flag;
    std::call_once(flag, []
        {
            Logger::CreateInstanceEx(g_logger);
            g_logger->Init(L"module.soldierbase");
            g_logger->SetProperty(logger::LoggerProperty::AsynLog, 1);
        });
#endif
}

EXTERN_C smart_result CreateObject(const GUID& guid, void** intf)
{
    InitLogger();
    
    CREATE_INSTANCE(IDemo,      guid, intf, Demo);
    CREATE_INSTANCE(ISoldierBase, guid, intf, SoldierBase);
    CREATE_INSTANCE(ILogger,    guid, intf, Logger);
    CREATE_INSTANCE(INetHelper, guid, intf, NetHelper);
    CREATE_INSTANCE(IDataBundle,guid, intf, DataBundle);
    CREATE_INSTANCE(IDataArray, guid, intf, DataArray);
    CREATE_INSTANCE(ITimer,     guid, intf, Timer);
    CREATE_INSTANCE(IPipeMgr,   guid, intf, PipeMgr);

    return err_code::e_nointerface;
}

