#pragma once

class SoldierBase :
    public Base::CObjectImpl<SoldierBase>,
    public ISoldierBase
{
    BEGIN_SINGLETON_OBJECT_MAP(SoldierBase)
        OBJECT_INTERFACE(ISoldierBase)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP()

    virtual smart_result __stdcall Initialize();

    virtual smart_result __stdcall UnInitialize();

private:
    bool init_{ false };

};

