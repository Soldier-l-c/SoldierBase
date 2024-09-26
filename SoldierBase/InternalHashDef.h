#pragma once

// openssl
#include <openssl/md5.h>
#include <openssl/sha.h>
#include "IInternalHash.h"
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

	typedef void* HASH_CTX;

	typedef int (*HASH_INIT) (HASH_CTX*);
	typedef int (*HASH_UPDATE) (HASH_CTX*, const void*, size_t);
	typedef int (*HASH_FINAL) (unsigned char*, HASH_CTX*);

	typedef IHashObjectPtr(*CREATE_HASH_OBJECT)(HASH_INIT pfn_init, HASH_UPDATE pfn_update, HASH_FINAL pfn_final);

	struct HASH_OBJECT_ITEM
	{
		NsHashData::HashType	type_{ NsHashData::HashType::HashTypeUnknown };
		size_t					length_{0};
		CREATE_HASH_OBJECT		fn_create_hash_object_{ nullptr };
		HASH_INIT		hash_init_{nullptr};
		HASH_UPDATE		hash_update_{ nullptr };
		HASH_FINAL		hash_final_{ nullptr };
		HASH_OBJECT_ITEM() = default;

		IHashObjectPtr CreateHashObject()
		{
			VERIFY_PTR_R(fn_create_hash_object_, nullptr);
			return fn_create_hash_object_(hash_init_, hash_update_, hash_final_);
		};

		HASH_OBJECT_ITEM(NsHashData::HashType type, size_t len, CREATE_HASH_OBJECT func, HASH_INIT init, HASH_UPDATE update, HASH_FINAL final_) :
			type_(type), length_(len), fn_create_hash_object_(func), hash_init_(init), hash_update_(update), hash_final_(final_)
		{

		}
	};
};