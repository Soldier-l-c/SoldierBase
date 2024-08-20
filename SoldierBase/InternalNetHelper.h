#pragma once

#ifndef CURL_STATICLIB
#ifdef WIN32
#ifdef _WIN64
#pragma comment(lib, "libcurl-x64.lib")
#else
#pragma comment(lib, "libcurl-x86.lib")
#endif
#endif
#else
#ifdef WIN32
#ifdef _WIN64
#pragma comment(lib, "libcurl-x64-release.lib")
#else
#pragma comment(lib, "libcurl-x86-release.lib")
#endif
#endif
#endif

class CurlReceiveStream
{
public:
	CurlReceiveStream(const std::wstring& file_path);
	CurlReceiveStream() = default;
	~CurlReceiveStream();

	bool OpenFile();

	size_t AddData(size_t size, void* data);

	uint8_t* Buffer();

	size_t ReceviedSize();

private:
	size_t AddToStream(size_t size, void* data);

	size_t AddToFile(size_t size, void* data);

private:
	bool write_to_file_{ false };

	std::wstring file_path_;

	size_t received_size_{ 0 };

	uint8_t* buffer_{ nullptr };

	FILE* hfile_{ nullptr };
};

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

struct CurlRequestBase
{
	IN std::string url;

	OUT int cur_code{ 0 };
	OUT std::string err_msg;
	OUT int http_code{ 0 };
	OUT NsNetHelper::NetError net_error{ NsNetHelper::SUCCESSED };
	OUT std::string response;

	virtual bool Valid();
};

struct HttpRequestInfo : CurlRequestBase
{
	IN std::string body;
	IN std::vector<std::string> headers;

	virtual bool Valid() override;
};

struct DownloadFileInfo : CurlRequestBase
{
	IN std::wstring file_path;

	virtual bool Valid() override;
};

using HttpRequestInfoPtr = std::shared_ptr<HttpRequestInfo>;
using DownloadFileInfoPtr = std::shared_ptr<DownloadFileInfo>;

using CurlRequestBasePtr = std::shared_ptr<CurlRequestBase>;

class InternalNetHelper
{
	SINGLE_INSTANCE(InternalNetHelper);
	
public:
	bool HttpRequest(const HttpRequestInfoPtr& sp_info);

	bool DownloadFile(const DownloadFileInfoPtr& sp_info);
};