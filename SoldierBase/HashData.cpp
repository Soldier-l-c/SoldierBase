#include "pch.h"
#include "HashData.h"
#include "InternalHash.h"

smart_result HashData::CalcHash(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashData& data)
{
    return InternalHash::instance().CalcHash(buffer, buffer_len, type, data);
}

smart_result HashData::CalcHashString(const char* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashStringData& data)
{
    NsHashData::HashData hash_data;
    auto res = CalcHash(buffer, buffer_len, type, hash_data);
    if (smart_failed(res))return res;

    const auto& hash_len = InternalHash::instance().GetHashLength(type);

    return DataToString(hash_data.data, hash_len, data);
}

smart_result HashData::DataToString(const uint8_t* buffer, const size_t buffer_len, NsHashData::HashStringData& str_data)
{
    return InternalHash::instance().DataToString(buffer, buffer_len, str_data);
}

smart_result HashData::StringToData(const char* buffer, const size_t buffer_len, NsHashData::HashData& data)
{
    return InternalHash::instance().StringToData(buffer, buffer_len, data);
}

smart_result __stdcall HashData::CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data)
{
    return smart_result();
}

smart_result __stdcall HashData::CalcFileHashString(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashStringData& data)
{
    return smart_result();
}