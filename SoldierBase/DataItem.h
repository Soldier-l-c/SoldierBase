#pragma once

namespace NsDataItem
{
	enum DataType
	{
		INT,
		INT64,
		UINT,
		UINT64,
		STRING,
		WSTRING,
		INTERFACE,
		POINTER,
		INVALID
	};
}

struct DataItem
{
	NsDataItem::DataType type{ NsDataItem::INVALID };
	union Data
	{
		int32_t int_val;
		int64_t int64_val;
		uint32_t uint_val;
		uint64_t uint64_val;

		wchar_t* pwstr_val;
		char* pstr_val;

		void* pointer_val;
		ISmartBase* intf;
	}data;

	DataItem();
	~DataItem();

public:
	template <typename T>
	bool GetData(T const* reqdata, NsDataItem::DataType reqtype)
	{
		if (this->type != reqtype)
		{
			return false;
		}

		memcpy((void*)reqdata, (void*)&data, sizeof(T));

		return true;
	}

	template <typename T>
	void SetData(const T& val, NsDataItem::DataType reqtype)
	{
		Clear();
		type = reqtype;
		memcpy((void*)&data, (void*)&val, sizeof(T));
	}

	smart_result  GetString(char* buffer, size_t* len);
	smart_result  GetWString(wchar_t* buffer, size_t* len);
	smart_result  GetInterface(ISmartBase** intf);

	smart_result  SetString(const char* buffer);
	smart_result  SetWString(const wchar_t* buffer);
	smart_result  SetInterface(ISmartBase* intf);

	void Clear();
};

using DataItemPtr = std::unique_ptr<DataItem>;
