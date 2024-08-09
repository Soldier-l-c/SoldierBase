#include "pch.h"
#include "InternalNetHelper.h"
#include "NetHelper.h"

smart_result __stdcall NetHelper::RquestUrlToFile(const char* url, const wchar_t* dest_file_path)
{
    if (!url || !dest_file_path)return err_code::e_invalidarg;

    return DownLoadFile(url, dest_file_path);
}

smart_result __stdcall NetHelper::DownLoadFile(const char* url, const wchar_t* dest_file_path)
{
    if (!url || !dest_file_path)return err_code::e_invalidarg;

    auto sp_info = std::make_shared<DownloadFileInfo>();
    sp_info->url = url;
    sp_info->file_path = dest_file_path;

    auto ret = InternalNetHelper::instance().DownloadFile(sp_info);

    return ret ? err_code::s_ok : err_code::e_fail;
}

smart_result __stdcall NetHelper::RequestBundle(IDataBundleRead* request, IDataBundleRead** response)
{
    if (!request || !response)return err_code::e_invalidarg;

    auto type = (NsNetHelper::NetReqType)(NsBundleHelper::GetInt(request, NsNetHelper::BundleKey::req_type));
    switch (type)
    {
    case NsNetHelper::HTTP:
        return InternalHttpRequest(request, response);
        break;
    case NsNetHelper::Download:
        return InternalDownloadFile(request, response);
        break;
    default:
        break;
    }

    return err_code::e_invalidarg;
}

smart_result NetHelper::InternalHttpRequest(IDataBundleRead* request, IDataBundleRead** response)
{
    HttpRequestInfoPtr sp_info;

    BundleToInternalHttpRequest(request, sp_info);

    auto ret = InternalNetHelper::instance().HttpRequest(sp_info);

    InternalResponseToBundle(sp_info, response);

    return ret ? err_code::s_ok : err_code::e_fail;
}

smart_result NetHelper::InternalDownloadFile(IDataBundleRead* request, IDataBundleRead** response)
{
    DownloadFileInfoPtr sp_info;

    BundleToInternalDownload(request, sp_info);

    auto ret = InternalNetHelper::instance().DownloadFile(sp_info);

    InternalResponseToBundle(sp_info, response);

    return ret ? err_code::s_ok : err_code::e_fail;
}

void NetHelper::BundleToInternalHttpRequest(IDataBundleRead* request, HttpRequestInfoPtr& sp_info)
{
    if (!sp_info)
    {
        sp_info = std::make_shared<HttpRequestInfo>();
    }

    sp_info->url = NsBundleHelper::GetString(request, NsNetHelper::BundleKey::req_url);
    sp_info->body = NsBundleHelper::GetString(request, NsNetHelper::BundleKey::req_body);
    
    IDataArrayReadPtr headers;
    request->GetInterface(NsNetHelper::BundleKey::req_headers, (ISmartBase**)&headers);
    if (headers)
    {
        for (size_t i = 0; i < headers->Count(); ++i)
        {
            sp_info->headers.push_back(NsDataArrayHelper::GetString(headers, i));
        }
    }
}

void NetHelper::BundleToInternalDownload(IDataBundleRead* request, DownloadFileInfoPtr& sp_info)
{
    if (!sp_info)
    {
        sp_info = std::make_shared<DownloadFileInfo>();
    }

    sp_info->url = NsBundleHelper::GetString(request, NsNetHelper::BundleKey::req_url);
    sp_info->file_path = NsBundleHelper::GetWString(request, NsNetHelper::BundleKey::req_file_path);
}

void NetHelper::InternalResponseToBundle(const CurlRequestBasePtr& sp_info, IDataBundleRead** response)
{
    auto bundle = NsBundleHelper::CreateObject<IDataBundlePtr>();
    (*response) = bundle;
    bundle->addref();

    bundle->PutInt(NsNetHelper::BundleKey::res_curl_code, sp_info->cur_code);
    bundle->PutInt(NsNetHelper::BundleKey::res_http_code, sp_info->http_code);
    bundle->PutInt(NsNetHelper::BundleKey::res_net_err_code, sp_info->net_error);
    bundle->PutString(NsNetHelper::BundleKey::res_curl_err_msg, sp_info->err_msg.c_str());
    bundle->PutString(NsNetHelper::BundleKey::res_response, sp_info->response.c_str());
}
