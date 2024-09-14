#pragma once
class HashData :
    public IHashData,
    public IHashFile,
    public Base::CObjectImpl<HashData>
{
    BEGIN_SINGLETON_OBJECT_MAP2(HashData, IHashData)
        OBJECT_INTERFACE(IHashData)
        OBJECT_INTERFACE(IHashFile)
        OBJECT_INTERFACE2(ISmartBase, IHashData)
    END_OBJECT_MAP();

    virtual smart_result __stdcall CalcHash(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashData& data) override;

    virtual smart_result __stdcall CalcHashString(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashStringData& data) override;

    virtual smart_result __stdcall DataToString(const uint8_t* buffer, const size_t buffer_len, NsHashData::HashStringData& str_data) override;

    virtual smart_result __stdcall StringToData(const char* buffer, const size_t buffer_len, NsHashData::HashData& data) override;

    virtual smart_result __stdcall CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data) override;

    virtual smart_result __stdcall CalcFileHashString(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashStringData& data) override;
};

