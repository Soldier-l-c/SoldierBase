#pragma once

// openssl
#include <openssl/md5.h>
#include <openssl/sha.h>

namespace NsHashDef
{
	using HASH_FUNC_TYPE = decltype(&MD5);

	struct HASH_FUNC_ITEM
	{
		NsHashData::HashType type{ NsHashData::HashTypeUnknown};
		size_t dest_len{ 0 };
		HASH_FUNC_TYPE func{ nullptr };

		HASH_FUNC_ITEM() = default;
		HASH_FUNC_ITEM(NsHashData::HashType type, size_t len, HASH_FUNC_TYPE func) :type(type), dest_len(len), func(func)
		{

		}
	};
};