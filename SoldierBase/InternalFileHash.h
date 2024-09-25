#pragma once
class InternalFileHash
{
	SINGLE_INSTANCE(InternalFileHash);

	void Init();

	smart_result CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data);

	bool CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, uint8_t* data, size_t* data_len);
};

