#pragma once

#include <curl/curl.h>

namespace shared::communication
{
	inline std::string m_szdata{};
	inline thread_local std::string m_szuseragent{"Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.101 Safari/537.36"};
	inline thread_local std::string m_szurl{};
	inline std::string m_szcookies{};
	inline std::string m_szresource{};
	inline thread_local std::string m_szresponse{};

	inline size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	};


	inline void post_url(CURL* pCurl)
	{
		m_szresponse = "";

		CURLcode res;

		curl_global_init(CURL_GLOBAL_ALL);

		if (!pCurl) {
			if (pCurl = curl_easy_init(); !pCurl)
			{
				curl_easy_cleanup(pCurl);
				curl_global_cleanup();
				return;
			}
		}

		curl_easy_setopt(pCurl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(pCurl, CURLOPT_URL, (m_szurl + m_szresource).c_str());
		curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &m_szresponse);
		curl_easy_setopt(pCurl, CURLOPT_USERAGENT, m_szuseragent);
		curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, m_szdata.c_str());
		//curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, "");
		curl_easy_setopt(pCurl, CURLOPT_COOKIEJAR, "-");
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, true);
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, true);

#ifdef _DEBUG
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, true);
		curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1);
#endif
		if (res = curl_easy_perform(pCurl);  res != CURLE_OK)
		{
			curl_easy_cleanup(pCurl);
			curl_global_cleanup();
			return;
		}

		//curl_easy_cleanup(pCurl);
		//curl_global_cleanup();

		m_szresponse = m_szresponse;

	}

	inline void set_data(const std::string& szName, const std::string& szVal)
	{
		m_szdata += '&';
		m_szdata += szName;
		m_szdata += '=';
		m_szdata += szVal;
	}
	inline void reset_data()
	{
		m_szdata = "";
	}
}