#include "pch.h"
#include "InternalFileHash.h"

void InternalFileHash::Init()
{
}

smart_result InternalFileHash::CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data)
{
	return smart_result();
}

bool InternalFileHash::CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, uint8_t* data, size_t* data_len)
{
	return false;
}
