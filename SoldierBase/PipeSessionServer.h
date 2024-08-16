#pragma once
#include "PipeSessionBase.h"
class PipeSessionServer :
    public PipeSessionBase,
    public Base::CObjectImpl<PipeSessionServer>
{
    BEGIN_OBJECT_MAP(PipeSessionServer)
        OBJECT_INTERFACE(IPipeSession)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

};

