#pragma once

class PipeMgr :
    public IPipeMgr,
    public Base::CObjectImpl<PipeMgr>
{
    BEGIN_SINGLETON_OBJECT_MAP(PipeMgr)
        OBJECT_INTERFACE(IPipeMgr)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

    virtual smart_result _stdcall CreatePipeServer(const wchar_t* pipe_name, IPipeServerCallback* server_callback);

    virtual smart_result _stdcall GetServer(const wchar_t* pipe_name, IPipeServer** server);

    virtual smart_result _stdcall CloseServer(const wchar_t* pipe_name);

    virtual smart_result _stdcall ConnectToServer(const wchar_t* pipe_name, IPipeClientCallback* client_callback);

public:
    void BroadcastData(const wchar_t* pip_name, void* data, int32_t len);

    void RemoveSession(const wchar_t* pip_name, const IPipeSessionPtr& session);

    void AddSession(const wchar_t* pip_name, const IPipeSessionPtr& session);

    smart_result InternalCreateServer(const wchar_t* pipe_name, IPipeServerCallback* server_callback);

    smart_result AsynWaitClientConnect(void* pipe_handle, const wchar_t* pipe_name, IPipeServerCallbackPtr server_callback);

private:
    using ServerList = std::map<std::wstring, IPipeServerPtr>;
    using SessionList = std::map<std::wstring, std::vector<IPipeSessionPtr>>;

    ServerList server_list_;
    SessionList session_list_;

    std::shared_mutex list_lock_;
};

