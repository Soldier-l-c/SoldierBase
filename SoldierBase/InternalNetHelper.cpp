#include "pch.h"
#include "InternalNetHelper.h"

static size_t callback(void* data, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	CurlReceiveStream* mem = (CurlReceiveStream*)userp;

	if (!mem)return 0;

	return mem->AddData(realsize, data);;
}

CurlReceiveStream::CurlReceiveStream(const std::wstring& file_path)
{
	write_to_file_ = true;
	file_path_ = file_path;
}

CurlReceiveStream::~CurlReceiveStream()
{
	if (write_to_file_ && hfile_)
	{
		fclose(hfile_);
		hfile_ = nullptr;
	}

	if (buffer_)
	{
		free(buffer_);
		buffer_ = nullptr;
	}
}

bool CurlReceiveStream::OpenFile()
{
	if (!write_to_file_)return true;

#pragma warning(push)
#pragma warning(disable : 4996)
	hfile_ = _wfopen(file_path_.c_str(), L"wb");
#pragma warning(pop)

	return !!hfile_;
}

size_t CurlReceiveStream::AddData(size_t size, void* data)
{
	return write_to_file_ ? AddToFile(size, data) : AddToStream(size, data);
}

uint8_t* CurlReceiveStream::Buffer()
{
	return buffer_;
}

size_t CurlReceiveStream::ReceviedSize()
{
	return received_size_;
}

size_t CurlReceiveStream::AddToStream(size_t size, void* data)
{
	if (size == 0 || !data)return 0;

	size_t realsize = size;

	uint8_t* ptr = (uint8_t*)realloc((void*)buffer_, received_size_ + realsize + 1);
	if (ptr == nullptr)
	{
		return 0;  /* out of memory! */
	}

	buffer_ = ptr;
	memcpy(&(buffer_[received_size_]), data, realsize);

	received_size_ += realsize;

	return realsize;
}

size_t CurlReceiveStream::AddToFile(size_t size, void* data)
{
	if (!hfile_ || size == 0 || !data)return 0;

	size_t written = fwrite(data, 1, size, hfile_);
	received_size_ += written;

	return written;
}

bool InternalNetHelper::HttpRequest(const HttpRequestInfoPtr& sp_info)
{
	if (!sp_info || !sp_info->Valid())return false;

	CURL* hcurl = curl_easy_init();
	if (hcurl == nullptr) 
	{
		sp_info->net_error = NsNetHelper::CURL_INIT_FAILED;
		return false;
	}

	curl_easy_setopt(hcurl, CURLOPT_URL, sp_info->url.c_str());

	CurlReceiveStream recv_item;

	// begin with https://,use ssl connection
	if (sp_info->url.size() > strlen("https://") && strncmp(sp_info->url.c_str(), "https://", strlen("https://")) == 0)
	{
		curl_easy_setopt(hcurl, CURLOPT_VERBOSE, 1L);
		
		curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYHOST, 0L);
	}

	curl_slist* header = nullptr;
	if (sp_info->headers.size() > 0) 
	{
		for (const auto& h : sp_info->headers)
		{
			header = curl_slist_append(header, h.c_str());
		}
		curl_easy_setopt(hcurl, CURLOPT_HTTPHEADER, header);
	}

	curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &recv_item);
	curl_easy_setopt(hcurl, CURLOPT_WRITEFUNCTION, callback);

	if (sp_info->body.size()) 
	{
		curl_easy_setopt(hcurl, CURLOPT_POST, 1L);

		curl_easy_setopt(hcurl, CURLOPT_POSTFIELDSIZE, sp_info->body.size());
		curl_easy_setopt(hcurl, CURLOPT_POSTFIELDS, sp_info->body.c_str());
	}

	//不设置代理
	curl_easy_setopt(hcurl, CURLOPT_PROXY, "");
	curl_easy_setopt(hcurl, CURLOPT_PROXYTYPE, -1L);

	curl_easy_setopt(hcurl, CURLOPT_CONNECTTIMEOUT_MS, 1000);
	curl_easy_setopt(hcurl, CURLOPT_TIMEOUT_MS, 3000);

	sp_info->cur_code = curl_easy_perform(hcurl);
	curl_easy_getinfo(hcurl, CURLINFO_RESPONSE_CODE, &sp_info->http_code);
	curl_easy_cleanup(hcurl);

	if (header) 
	{
		curl_slist_free_all(header);
	}

	if (sp_info->cur_code != CURLE_OK)
	{
		sp_info->err_msg = curl_easy_strerror((CURLcode)sp_info->cur_code);
		sp_info->net_error = NsNetHelper::CURL_PERFORM_FAILED;

		return false;
	}
	else if (sp_info->http_code == 200)
	{
		if (recv_item.Buffer() && recv_item.ReceviedSize()>0) 
		{
			sp_info->response = std::string((char*)recv_item.Buffer(), recv_item.ReceviedSize());
		}
	}

	return true;
}

bool InternalNetHelper::DownloadFile(const DownloadFileInfoPtr& sp_info)
{
	if (!sp_info || !sp_info->Valid())return false;

	CURL* hcurl = curl_easy_init();
	if (hcurl == nullptr)
	{
		sp_info->net_error = NsNetHelper::CURL_INIT_FAILED;
		return false;
	}

	curl_easy_setopt(hcurl, CURLOPT_URL, sp_info->url.c_str());

	std::filesystem::path path(sp_info->file_path);
	auto parent = path.parent_path();
	std::error_code ec;
	if (!std::filesystem::exists(parent, ec)) 
	{
		std::filesystem::create_directories(parent, ec);
	}

	CurlReceiveStream recv_item(sp_info->file_path);
	if (!recv_item.OpenFile()) 
	{
		curl_easy_cleanup(hcurl);
		sp_info->net_error = NsNetHelper::OPEN_FILE_FAILED;
		return false;
	}

	// 不校验https证书
	curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(hcurl, CURLOPT_SSL_VERIFYHOST, 0L);

	// Curl_easy_setopt(pCurlHandle, CURLOPT_HTTPHEADER, m_CustomHeader);
	curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &recv_item);
	curl_easy_setopt(hcurl, CURLOPT_WRITEFUNCTION, callback);

	//不设置代理
	curl_easy_setopt(hcurl, CURLOPT_PROXY, "");
	curl_easy_setopt(hcurl, CURLOPT_PROXYTYPE, -1);
	
	curl_easy_setopt(hcurl, CURLOPT_CONNECTTIMEOUT_MS, 60 * 1000);
	curl_easy_setopt(hcurl, CURLOPT_TIMEOUT_MS, 60 * 1000);

	sp_info->cur_code = curl_easy_perform(hcurl);
	curl_easy_getinfo(hcurl, CURLINFO_RESPONSE_CODE, &sp_info->http_code);
	curl_easy_cleanup(hcurl);

	if (sp_info->cur_code != CURLE_OK) 
	{
		sp_info->err_msg = curl_easy_strerror((CURLcode)sp_info->cur_code);
		sp_info->net_error = NsNetHelper::CURL_PERFORM_FAILED;
		return false;
	}

	return sp_info->http_code == 200;
}

bool CurlRequestBase::Valid()
{
	return !url.empty();
}

bool DownloadFileInfo::Valid()
{
	return CurlRequestBase::Valid() && !file_path.empty();
}

bool HttpRequestInfo::Valid()
{
	return CurlRequestBase::Valid();
}
