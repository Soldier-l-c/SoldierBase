#include "pch.h"
#include "DataItem.h"
using namespace NsDataItem;
using namespace NsDataArray;

DataItem::DataItem()
{
	memset(&data, 0, sizeof(data));
}

DataItem::~DataItem()
{
	Clear();
}

smart_result DataItem::GetString(char* buffer, size_t* len)
{
	if (type != DataType::STRING)
	{
		return err_code::e_type_error;
	}

	auto reallen = helper::String::get_length(data.pstr_val) + 1;

	if (*len < reallen)
	{
		*len = reallen;
		return ::err_code::e_insufficient_buffer;
	}

	if (!buffer)return ::err_code::e_invalidarg;;

	if (data.pstr_val)
	{
		strcpy_s(buffer, reallen, data.pstr_val);
	}
	else
	{
		buffer[0] = 0;
	}

	return err_code::s_ok;

	return smart_result();
}

smart_result DataItem::GetWString(wchar_t* buffer, size_t* len)
{
	if (type != DataType::WSTRING)
	{
		return err_code::e_type_error;
	}

	auto reallen = helper::String::get_length(data.pwstr_val) + 1;

	if (*len < reallen)
	{
		*len = reallen;
		return ::err_code::e_insufficient_buffer;
	}

	if (!buffer)return ::err_code::e_invalidarg;;

	if (data.pwstr_val)
	{
		wcscpy_s(buffer, reallen, data.pwstr_val);
	}
	else
	{
		buffer[0] = 0;
	}

	return err_code::s_ok;
}

smart_result DataItem::GetInterface(ISmartBase** intf)
{
	if (type != DataType::INTERFACE)
	{
		return err_code::e_type_error;
	}

	(*intf) =data.intf;
	if (data.intf)
	{
		(*intf)->addref();
	}

	return err_code::s_ok;
}

smart_result DataItem::SetString(const char* buffer)
{
	Clear();

	type = DataType::STRING;

	auto reallen = helper::String::get_length(buffer) + 1;

	data.pstr_val = new char[reallen];
	if (buffer)
	{
		strcpy_s(data.pstr_val, reallen, buffer);
	}
	else
	{
		data.pstr_val[0] = 0;
	}

	return err_code::s_ok;
}

smart_result DataItem::SetWString(const wchar_t* buffer)
{
	Clear();

	type = DataType::WSTRING;

	auto reallen = helper::String::get_length(buffer) + 1;

	data.pwstr_val = new wchar_t[reallen];
	if (buffer)
	{
		wcscpy_s(data.pwstr_val, reallen, buffer);
	}
	else
	{
		data.pwstr_val[0] = 0;
	}

	return err_code::s_ok;
}

smart_result DataItem::SetInterface(ISmartBase* intf)
{
	Clear();

	type = DataType::INTERFACE;
	data.intf = intf;

	if (intf)
	{
		intf->addref();
	}

	return err_code::s_ok;
}

void DataItem::Clear()
{
	switch (type)
	{
	case NsDataItem::INT:
	case NsDataItem::INT64:
	case NsDataItem::UINT:
	case NsDataItem::UINT64:
	case NsDataItem::INVALID:
	case NsDataItem::POINTER:
		break;
	case NsDataItem::STRING:
		if (data.pstr_val)
		{
			delete[]data.pstr_val;
		}
		break;
	case NsDataItem::WSTRING:
		if (data.pwstr_val)
		{
			delete[]data.pwstr_val;
		}
		break;
	case NsDataItem::INTERFACE:
		if (data.intf)
		{
			data.intf->release();
		}
		break;
	default:
		break;
	}
	memset(&data, 0, sizeof(data));
	type = NsDataItem::DataType::INVALID;
}
