#pragma once

template<int32_t TYPE, typename HASH_CTX>
inline HashObject<TYPE, HASH_CTX>::HashObject()
{
	memset(&ctx_, 0, sizeof(ctx_));
}

template<int32_t TYPE, typename HASH_CTX>
inline int32_t HashObject<TYPE, HASH_CTX>::Init()
{
	return pfn_init_ ? pfn_init_(&ctx_) : -1;
}

template<int32_t TYPE, typename HASH_CTX>
inline int32_t HashObject<TYPE, HASH_CTX>::Update(const void* data, size_t len)
{
	return pfn_update_ ? pfn_update_(&ctx_, data, len) : -1;
}

template<int32_t TYPE, typename HASH_CTX>
inline int32_t HashObject<TYPE, HASH_CTX>::Final(NsHashData::HashData& data)
{
	return  pfn_final_ ? pfn_final_(data.data, &ctx_) : -1;
}

template<int32_t TYPE, typename HASH_CTX>
inline IHashObjectPtr HashObject<TYPE, HASH_CTX>::CreateHashObject(HASH_INIT pfn_init, HASH_UPDATE pfn_update, HASH_FINAL pfn_final)
{
	VERIFY_PTR_R(pfn_init, nullptr);
	VERIFY_PTR_R(pfn_update, nullptr);
	VERIFY_PTR_R(pfn_final, nullptr);

	auto ret = std::make_shared<HashObject<TYPE, HASH_CTX>>();

	ret->pfn_init_ = pfn_init;
	ret->pfn_update_ = pfn_update;
	ret->pfn_final_ = pfn_final;

	return ret;
}
