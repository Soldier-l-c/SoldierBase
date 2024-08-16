#pragma once
#include "PipeSessionBase.h"

class PipeSessionClient :
    public PipeSessionBase,
    public Base::CObjectImpl<PipeSessionClient>
{
    BEGIN_OBJECT_MAP(PipeSessionClient)
        OBJECT_INTERFACE(IPipeSession)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

    PipeSessionClient(IPipeClientCallback* callback);

protected:
    virtual void IniternalClose(uint32_t err_code);

private:
    IPipeClientCallbackPtr  callback_;
};

