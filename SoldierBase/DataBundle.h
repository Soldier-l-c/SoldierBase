#pragma once
#include "DataItem.h"

class DataBundle :
	public Base::CObjectImpl<DataBundle>,
	public IDataBundle
{
	BEGIN_OBJECT_MAP(DataBundle)
		OBJECT_INTERFACE(IDataBundle)
		OBJECT_INTERFACE(IDataBundleRead)
		OBJECT_INTERFACE(ISmartBase)
	END_OBJECT_MAP();

	virtual smart_result __stdcall GetString(const char* key, char* buffer, size_t* len);
	virtual smart_result __stdcall GetWString(const char* key, wchar_t* buffer, size_t* len);
	virtual smart_result __stdcall GetInt(const char* key, int32_t* val);
	virtual smart_result __stdcall GetInt64(const char* key, int64_t* val);
	virtual smart_result __stdcall GetUInt(const char* key, uint32_t* val);
	virtual smart_result __stdcall GetUInt64(const char* key, uint64_t* val);
	virtual smart_result __stdcall GetPointer(const char* key, void** ptr);
	virtual smart_result __stdcall GetInterface(const char* key, ISmartBase** intf);

	virtual uint32_t	 __stdcall Count();

	virtual uint32_t	 __stdcall Remove(const char* key);
	virtual void		 __stdcall RemoveAll();

	virtual smart_result __stdcall PutString(const char* key, const char* str);
	virtual smart_result __stdcall PutWString(const char* key, const wchar_t* str);
	virtual smart_result __stdcall PutInt(const char* key, int32_t val);
	virtual smart_result __stdcall PutInt64(const char* key, int64_t val);
	virtual smart_result __stdcall PutUInt(const char* key, uint32_t val);
	virtual smart_result __stdcall PutUInt64(const char* key, uint64_t val);
	virtual smart_result __stdcall PutPointer(const char* key, void* ptr);
	virtual smart_result __stdcall PutInterface(const char* key, ISmartBase* ptr);

private:
	std::map<std::string, DataItemPtr> data_list_;
};