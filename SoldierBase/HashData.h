#pragma once
class HashData :
    public IHashData,
    public Base::CObjectImpl<HashData>
{
    BEGIN_SINGLETON_OBJECT_MAP(HashData)
        OBJECT_INTERFACE(IHashData)
        OBJECT_INTERFACE(ISmartBase)
    END_OBJECT_MAP();

    virtual smart_result CalcHash(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashData& data) override;

    virtual smart_result CalcHashString(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashStringData& data) override;

    virtual smart_result DataToString(const uint8_t* buffer, const size_t buffer_len, NsHashData::HashStringData& str_data) override;

    virtual smart_result StringToData(const char* buffer, const size_t buffer_len, NsHashData::HashData& data) override;
};

