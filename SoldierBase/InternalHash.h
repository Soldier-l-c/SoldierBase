#pragma once
#include "InternalHashDef.h"

class InternalHash
{
	SINGLE_INSTANCE(InternalHash);

	void Init();

	smart_result DataToString(const uint8_t* b, size_t len, NsHashData::HashStringData& data);

	smart_result StringToData(const char* buffer, size_t buffer_len, NsHashData::HashData& data);

	smart_result CalcHash(const void* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashData& data);

	bool CalcHash(const uint8_t* buffer, const size_t buffer_len, NsHashData::HashType type, uint8_t* data, size_t* data_len);

	size_t GetHashLength(NsHashData::HashType type);

private:
	const NsHashDef::HASH_FUNC_ITEM* QueryFuncItem(NsHashData::HashType type);

private:
	std::vector<NsHashDef::HASH_FUNC_ITEM>hash_cacl_list_;
};

