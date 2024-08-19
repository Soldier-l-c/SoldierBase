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

    PipeSessionServer(IPipeServerCallback* callback, const stream_handle_ptr& pipe_handle, const std::wstring& pipe_name);

    virtual void OnDataRecived(uint8_t* data, int32_t len) override;

    virtual void OnError(uint32_t err_code);

protected:
    virtual void IniternalClose(uint32_t err_code) override;

private:
    IPipeServerCallbackPtr callback_;

};

