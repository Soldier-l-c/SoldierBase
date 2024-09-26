#pragma once
#include "InternalHashDef.h"
class InternalFileHash
{
	SINGLE_INSTANCE(InternalFileHash);

	void Init();

	smart_result CalcFileHash(const wchar_t* file_path, NsHashData::HashType type, NsHashData::HashData& data);
};

