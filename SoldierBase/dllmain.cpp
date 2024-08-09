// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

#include "Logger.h"
#include "NetHelper.h"
#include "DataBundle.h"
#include "DataArray.h"

BOOL APIENTRY DllMain( HMODULE hModule,
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

class Demo :
    public Base::CObjectImpl<Demo>,
    public IDemo
{
public:
    Demo() {};
    virtual ~Demo() { LOG(INFO) << "~Demo"; };

public:
    BEGIN_OBJECT_MAP(Demo)
        OBJECT_INTERFACE(IDemo)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

public:
    virtual const char* __stdcall Test()override { return "asdadsad"; };

};

EXTERN_C smart_result CreateObject(const GUID& guid, void** intf)
{
    CREATE_INSTANCE(IDemo, guid, intf, Demo);
    CREATE_INSTANCE(ILogger, guid, intf, Logger);
    CREATE_INSTANCE(INetHelper, guid, intf, NetHelper);
    CREATE_INSTANCE(IDataBundle, guid, intf, DataBundle);
    CREATE_INSTANCE(IDataArray, guid, intf, DataArray);

    return err_code::e_nointerface;
}

