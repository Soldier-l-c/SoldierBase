#pragma once
#include <base/IDataArray.h>
#include "DataItem.h"


class DataArray : 
	public Base::CObjectImpl<DataArray>,
	public IDataArray
{
	BEGIN_OBJECT_MAP(DataArray)
		OBJECT_INTERFACE(IDataArray)
		OBJECT_INTERFACE(IDataArrayRead)
		OBJECT_INTERFACE(ISmartBase)
	END_OBJECT_MAP();

	virtual smart_result __stdcall GetInt(uint32_t index, int32_t* val);
	virtual smart_result __stdcall GetUInt(uint32_t index, uint32_t* val);
	virtual smart_result __stdcall GetInt64(uint32_t index, int64_t* val);
	virtual smart_result __stdcall GetUInt64(uint32_t index, uint64_t* val);

	virtual smart_result __stdcall GetString(uint32_t index, char* buffer, int32_t* len);
	virtual smart_result __stdcall GetWString(uint32_t index, wchar_t* buffer, int32_t* len);
	virtual smart_result __stdcall GetInterface(uint32_t index, ISmartBase** intf);
	virtual smart_result __stdcall GetPointer(uint32_t index, void** intf);

	virtual uint32_t	 __stdcall Count();

	virtual smart_result __stdcall Remove(uint32_t index);

	virtual void		 __stdcall RemoveAll();

	virtual smart_result __stdcall AddInt(int32_t val);
	virtual smart_result __stdcall AddUInt(uint32_t val);
	virtual smart_result __stdcall AddInt64(int64_t val);
	virtual smart_result __stdcall AddUInt64(uint64_t val);

	virtual smart_result __stdcall AddString(const char* buffer);
	virtual smart_result __stdcall AddWString(const wchar_t* buffer) override;
	virtual smart_result __stdcall AddInterface(ISmartBase* intf);
	virtual smart_result __stdcall AddPointer(void* intf);

private:
	std::vector<DataItemPtr>data_list_;
};

