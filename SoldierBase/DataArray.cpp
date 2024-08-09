#include "pch.h"
#include "DataArray.h"
using namespace NsDataArray;
using namespace NsDataItem;

#define CheckIndex(index, re_type)\
	if (index > data_list_.size())return err_code::e_outof_index;\
	if (data_list_[index]->type != re_type)return err_code::e_type_error;

smart_result __stdcall DataArray::GetInt(uint32_t index, int32_t* val)
{
	if (!val)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::INT);

	data_list_[index]->GetData(val, DataType::INT);

	return ::err_code::s_ok;
}

smart_result __stdcall DataArray::GetUInt(uint32_t index, uint32_t* val)
{
	if (!val)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::UINT);

	data_list_[index]->GetData(val, DataType::UINT);

	return ::err_code::s_ok;
}

smart_result __stdcall DataArray::GetInt64(uint32_t index, int64_t* val)
{
	if (!val)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::INT64);

	data_list_[index]->GetData(val, DataType::INT64);

	return ::err_code::s_ok;
}

smart_result __stdcall DataArray::GetUInt64(uint32_t index, uint64_t* val)
{
	if (!val)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::UINT64);

	data_list_[index]->GetData(val, DataType::UINT64);

	return ::err_code::s_ok;
}

smart_result __stdcall DataArray::GetString(uint32_t index, char* buffer, int32_t* len)
{
	if (!len)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::STRING);

	return data_list_[index]->GetString(buffer, len);
}

smart_result __stdcall DataArray::GetWString(uint32_t index, wchar_t* buffer, int32_t* len)
{
	if (!len)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::WSTRING);

	return data_list_[index]->GetWString(buffer, len);
}

smart_result __stdcall DataArray::GetInterface(uint32_t index, ISmartBase** intf)
{
	if (!intf)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::INTERFACE);

	return data_list_[index]->GetInterface(intf);
}

smart_result __stdcall DataArray::GetPointer(uint32_t index, void** intf)
{
	if (!intf)return ::err_code::e_invalidarg;

	CheckIndex(index, DataType::POINTER);

	data_list_[index]->GetData(intf, DataType::POINTER);

	return err_code::s_ok;
}

uint32_t __stdcall DataArray::Count()
{
	return data_list_.size();
}

smart_result __stdcall DataArray::Remove(uint32_t index)
{
	if (index > data_list_.size())return err_code::e_outof_index;

	data_list_.erase(data_list_.begin()+index);

	return err_code::s_ok;
}

void __stdcall DataArray::RemoveAll()
{
	data_list_.clear();
}

smart_result __stdcall DataArray::AddInt(int32_t val)
{
	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::INT);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddUInt(uint32_t val)
{
	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::UINT);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddInt64(int64_t val)
{
	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::INT64);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddUInt64(uint64_t val)
{
	auto item = std::make_unique<DataItem>();
	item->SetData(val, DataType::UINT64);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddString(const char* buffer)
{
	if (!buffer)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetString(buffer);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddWString(const wchar_t* buffer)
{
	if(!buffer)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetWString(buffer);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddInterface(ISmartBase* intf)
{
	if (!intf)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetInterface(intf);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}

smart_result __stdcall DataArray::AddPointer(void* intf)
{
	if (!intf)return err_code::e_invalidarg;

	auto item = std::make_unique<DataItem>();
	item->SetData(intf, DataType::POINTER);
	data_list_.push_back(std::move(item));

	return err_code::s_ok;
}