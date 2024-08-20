#pragma once
#include <base/INetHelper.h>
#include <base/IDataBundle.h>
#include "InternalNetHelper.h"

class NetHelper :public INetHelper, public Base::CObjectImpl<NetHelper>
{
	BEGIN_SINGLETON_OBJECT_MAP(NetHelper)
		OBJECT_INTERFACE(INetHelper)
		OBJECT_INTERFACE(ISmartBase)
		END_OBJECT_MAP();


	virtual smart_result __stdcall RquestUrlToFile(const char* url, const wchar_t* dest_file_path) override;

	virtual smart_result __stdcall DownLoadFile(const char* url, const wchar_t* dest_file_path) override;

	virtual smart_result __stdcall RequestBundle(IDataBundleRead* request, IDataBundleRead** response) override;

private:
	smart_result InternalHttpRequest(IDataBundleRead* request, IDataBundleRead** response);
	smart_result InternalDownloadFile(IDataBundleRead* request, IDataBundleRead** response);

	void BundleToInternalHttpRequest(IDataBundleRead* request, HttpRequestInfoPtr& sp_info);
	void BundleToInternalDownload(IDataBundleRead* request, DownloadFileInfoPtr& sp_info);

	void InternalResponseToBundle(const CurlRequestBasePtr& sp_info, IDataBundleRead** response);

};