#pragma once
#include "IInternalHash.h"

template <int32_t TYPE, typename HASH_CTX>
class HashObject :
    public IInternalHashObject
{
public:
	__inline  HashObject();

	__inline  int32_t Init() override;

	__inline  int32_t Update(const void* data, size_t len) override;

	__inline  int32_t Final(NsHashData::HashData& data) override;

public:
	typedef int (*HASH_INIT) (HASH_CTX*);
	typedef int (*HASH_UPDATE) (HASH_CTX*, const void*, size_t);
	typedef int (*HASH_FINAL) (unsigned char*, HASH_CTX*);

	__inline static IHashObjectPtr CreateHashObject(HASH_INIT pfn_init, HASH_UPDATE pfn_update, HASH_FINAL pfn_final);

public:
	HASH_INIT pfn_init_{ nullptr };

	HASH_UPDATE pfn_update_{ nullptr };

	HASH_FINAL pfn_final_{ nullptr };

	HASH_CTX ctx_;
};
#include "HashObject.inl"
