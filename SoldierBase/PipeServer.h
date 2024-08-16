#pragma once

class PipeServer :
    public IPipeServer,
    public Base::CObjectImpl<PipeServer>
{
    BEGIN_OBJECT_MAP(PipeServer)
        OBJECT_INTERFACE(IPipeServer)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

    virtual void _stdcall BroadcastData(void* data, int32_t len);

};

