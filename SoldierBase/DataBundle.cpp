#include "pch.h"
#include "DataBundle.h"
using namespace NsDataItem;

#define CheckExists(key, type_)\
	auto iter = data_list_.find(key);\
	if (iter == data_list_.end() || iter->second->type != type_)\
	{\
		return err_code::e_invalidarg;\
	}\

#define SetValue(key, value)\
	auto iter = data_list_.find(key);\
	if (iter ==data_list_.end())\
	{\
		data_list_.insert({key, std::move(value)});\
	}\
	else \
	{\
		iter->second = std::move(value);\
	}

smart_result __stdcall DataBundle::GetString(const char* key, char* buffer, int32_t* len)
{
	if (!key || !len)return err_code::e_invalidarg;

	CheckExists(key, DataType::STRING);

	return iter->second->GetString(buffer, len);
}

smart_result __stdcall DataBundle::GetWString(const char* key, wchar_t* buffer, int32_t* len)
{
	if (!key || !len)return err_code::e_invalidarg;

	CheckExists(key, DataType::WSTRING);

	return iter->second->GetWString(buffer, len);
}

smart_result __stdcall DataBundle::GetInt(const char* key, int32_t* val)
{
	if (!key || !val)return err_code::e_invalidarg;

	CheckExists(key, DataType::INT);

	iter->second->GetData(val, DataType::INT);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::GetInt64(const char* key, int64_t* val)
{
	if (!key || !val)return err_code::e_invalidarg;

	CheckExists(key, DataType::INT64);

	iter->second->GetData(val, DataType::INT64);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::GetUInt(const char* key, uint32_t* val)
{
	if (!key || !val)return err_code::e_invalidarg;

	CheckExists(key, DataType::UINT);

	iter->second->GetData(val, DataType::UINT);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::GetUInt64(const char* key, uint64_t* val)
{
	if (!key || !val)return err_code::e_invalidarg;

	CheckExists(key, DataType::UINT64);

	iter->second->GetData(val, DataType::UINT64);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::GetPointer(const char* key, void** ptr)
{
	if (!key || !ptr)return err_code::e_invalidarg;

	CheckExists(key, DataType::POINTER);

	iter->second->GetData(ptr, DataType::POINTER);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::GetInterface(const char* key, ISmartBase** intf)
{
	if (!key || !intf)return err_code::e_invalidarg;

	CheckExists(key, DataType::INTERFACE);

	return iter->second->GetInterface(intf);
}

smart_result __stdcall DataBundle::PutString(const char* key, const char* str)
{
	if (!key || !str)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetString(str);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutWString(const char* key, const wchar_t* str)
{
	if (!key || !str)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetWString(str);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutInt(const char* key, int32_t val)
{
	if (!key)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::INT);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutInt64(const char* key, int64_t val)
{
	if (!key)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::INT64);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutUInt(const char* key, uint32_t val)
{
	if (!key)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::UINT);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutUInt64(const char* key, uint64_t val)
{
	if (!key)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::UINT64);
	SetValue(key, item);
	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutPointer(const char* key, void* ptr)
{
	if (!key || !ptr)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(ptr, DataType::POINTER);
	SetValue(key, item);

	return err_code::s_ok;
}

smart_result __stdcall DataBundle::PutInterface(const char* key, ISmartBase* ptr)
{
	if (!key || !ptr)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetInterface(ptr);
	SetValue(key, item);

	return err_code::s_ok;
}
