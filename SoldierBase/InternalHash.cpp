#include "pch.h"
#include "InternalHash.h"
#include "HashObject.h"

using namespace NsHashData;

typedef HashObject<HashTypeMD5, MD5_CTX>		CMD5Object;
typedef HashObject<HashTypeSHA1, SHA_CTX>		CSHA1Object;
typedef HashObject<HashTypeSHA224, SHA256_CTX>	CSHA224Object;
typedef HashObject<HashTypeSHA256, SHA256_CTX>	CSHA256Object;
typedef HashObject<HashTypeSHA384, SHA512_CTX>	CSHA384Object;
typedef HashObject<HashTypeSHA512, SHA512_CTX>	CSHA512Object;

#ifndef SHA1_DIGEST_LENGTH
#define SHA1_DIGEST_LENGTH SHA_DIGEST_LENGTH
#endif

#define HASH_OBJECT_ITEM_MAKER(item)\
	{HashType##item, item##_DIGEST_LENGTH, \
	(NsHashDef::CREATE_HASH_OBJECT)&C##item##Object::CreateHashObject,\
	(NsHashDef::HASH_INIT) item##_Init,\
	(NsHashDef::HASH_UPDATE) item##_Update,\
	(NsHashDef::HASH_FINAL) item##_Final}

#define HASH_FUNC_ITEM_MAKER(item)\
	{HashType##item,item##_DIGEST_LENGTH,item}

char DigitalToHexChar(uint8_t v, bool up)
{
	if (v < 0 || v >= 0x10) return 0;

	const auto temp = up ? ("0123456789ABCDEF") : ("0123456789abcdef");

	return temp[v];
}

int HexCharToDigit(char ch)
{
	if (ch >= '0' && ch <= '9') return ch - ('0');

	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	else
		return -1;
}

template <typename Ch>
void ByteToHexStr(uint8_t c, Ch* str, bool up = false)
{
	*str++ = DigitalToHexChar(c >> 4, up);
	*str = DigitalToHexChar(c & 0x0F, up);
}

smart_result InternalHash::DataToString(const uint8_t* b, size_t len, HashStringData& data)
{
	VERIFY_PTR(b);
	if (len > MAX_HASH_DATA_LENGTH)return err_code::e_invalidarg;

	InitHashData(data);

	auto p = data.data;
	while (len--)
	{
		ByteToHexStr(*(b++), p);
		p += 2;
	}

	*p = 0;

	return err_code::s_ok;
}

smart_result InternalHash::StringToData(const char* buffer, size_t buffer_len, NsHashData::HashData& data)
{
	VERIFY_PTR(buffer);
	if (buffer_len > MAX_HASH_STRING_DATA_LENGTH)
		return err_code::e_invalidarg;

	InitHashData(data);

	for (size_t i = 0; i < buffer_len; i+=2)
	{
		uint8_t hi = HexCharToDigit(buffer[i]);
		uint8_t lo = HexCharToDigit(buffer[i + 1]);

		if (hi == -1 || lo == -1)
			return err_code::e_fail;

		data.data[i >> 1] = (hi << 4) | lo;
	}

	data.length = buffer_len >> 1;

	return err_code::error_success;
}

smart_result InternalHash::CalcHash(const void* buffer, const size_t buffer_len, NsHashData::HashType type, NsHashData::HashData& data)
{
	InitHashData(data);

	data.length = sizeof(data.data);
	auto res = CalcHash(static_cast<const uint8_t*>(buffer), buffer_len, type, data.data, &data.length);
	
	return res ? err_code::s_ok : err_code::e_fail;
}

bool InternalHash::CalcHash(const uint8_t* buffer, const size_t buffer_len, NsHashData::HashType type, uint8_t* data, size_t* data_len)
{
	VERIFY_PTR(buffer);
	VERIFY_PTR(data);
	VERIFY_PTR(data_len);

	const auto& item = QueryFuncItem(type);
	if (!item || *data_len < item->dest_len)
		return false;

	item->func((const uint8_t*)buffer, buffer_len, data);
	*data_len = item->dest_len;

	return true;
}

size_t InternalHash::GetHashLength(NsHashData::HashType type)
{
	const auto& item = QueryFuncItem(type);
	if (!item)
		return 0;

	return item->dest_len;
}

NsHashDef::HASH_OBJECT_ITEM* InternalHash::QueryObjectItem(NsHashData::HashType type)
{
	NsHashDef::HASH_OBJECT_ITEM* res{ nullptr };

	for (size_t i = 0; i < object_item_list_.size(); ++i)
	{
		if (object_item_list_[i].type_ == type)
		{
			res = &object_item_list_[i];
		}
	}

	return  res;
}

void InternalHash::Init()
{
	hash_cacl_list_ = { 
		HASH_FUNC_ITEM_MAKER(MD5),
		HASH_FUNC_ITEM_MAKER(SHA1),
		HASH_FUNC_ITEM_MAKER(SHA224),
		HASH_FUNC_ITEM_MAKER(SHA256),
		HASH_FUNC_ITEM_MAKER(SHA384),
		HASH_FUNC_ITEM_MAKER(SHA512),
	};

	object_item_list_ = {
		HASH_OBJECT_ITEM_MAKER(MD5),
		HASH_OBJECT_ITEM_MAKER(SHA1),
		HASH_OBJECT_ITEM_MAKER(SHA224),
		HASH_OBJECT_ITEM_MAKER(SHA256),
		HASH_OBJECT_ITEM_MAKER(SHA384),
		HASH_OBJECT_ITEM_MAKER(SHA512),
	};
}

const NsHashDef::HASH_FUNC_ITEM* InternalHash::QueryFuncItem(NsHashData::HashType type)
{
	NsHashDef::HASH_FUNC_ITEM* res{ nullptr };

	for (size_t i = 0; i< hash_cacl_list_.size(); ++i)
	{
		if (hash_cacl_list_[i].type == type)
		{
			res = &hash_cacl_list_[i];
		}
	}
	
	return  res;
}
