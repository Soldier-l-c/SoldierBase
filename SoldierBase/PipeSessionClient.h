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

    PipeSessionClient(IPipeClientCallback* callback, void* pipe_handle, const std::wstring& pipe_name);

    virtual void OnDataRecived(uint8_t* data, int32_t len) override;

protected:
    virtual void IniternalClose(uint32_t err_code) override;

private:
    IPipeClientCallbackPtr  callback_;
};

