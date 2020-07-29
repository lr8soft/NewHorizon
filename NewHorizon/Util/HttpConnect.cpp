#include "HttpConnect.h"
#include "LogUtil.hpp"
#include <sstream>
#include <curl/curl.h>
bool HttpConnect::haveGlobalInit = false;
void HttpConnect::releaseGlobalEnov()
{
	if (haveGlobalInit)
	{
		curl_global_cleanup();
		haveGlobalInit = false;
	}

}
std::string HttpConnect::readFromUrl(const std::string url)
{

	CURL* curlHandle = curl_easy_init();
	if (curlHandle == nullptr)
	{
		LogUtil::printError("Fail to create curl handle.");
	}

	std::stringstream out;
	curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &writeData);

	//bind data output
	curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &out);

	//start request
	curl_easy_perform(curlHandle);

	//clean curl
	curl_easy_cleanup(curlHandle);
	return out.str();
}
HttpConnect::HttpConnect()
{
	if (!haveGlobalInit)
	{
#ifdef _WIN64
		curl_global_init(CURL_GLOBAL_WIN32);
#else
		curl_global_init(CURL_GLOBAL_SSL);
#endif
		haveGlobalInit = true;
	}
}

size_t HttpConnect::writeData(void* buffer, size_t size, size_t nmemb, void* userp)
{
	//convert void* to char* and read length
	std::string data((char*)buffer, size * nmemb);
	*((std::stringstream*) userp) << data << std::endl;
	return  size * nmemb;
}
