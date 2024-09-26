#pragma once

struct IInternalHashObject
{
	virtual int32_t Init() = 0;

	virtual int32_t Update(const void* data, size_t len) = 0;

	virtual int32_t Final(NsHashData::HashData& data) = 0;
};
using IHashObjectPtr = std::shared_ptr<IInternalHashObject>;