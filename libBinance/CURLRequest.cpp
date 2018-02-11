#include "CURLRequest.h"
#include <iostream>
#include <sstream>
#include "curl/curl.h"

CURLRequest::CURLRequest()
{
	// Initialise CURL library
	curl_global_init(CURL_GLOBAL_ALL);
	//std::cout << "Curl version:" << curl_version() << std::endl;
	m_pCurlHandle = curl_easy_init();

	// Create memory buffer for request
	m_BufferSize = 1024;
	m_pBuffer = new char[m_BufferSize];
}

CURLRequest::~CURLRequest()
{
	// cleanup curl stuff
	curl_easy_cleanup(m_pCurlHandle);
	curl_global_cleanup();

	// Free the request buffer
	delete m_pBuffer;
}

std::string CURLRequest::ping()
{
	return request("https://api.binance.com/api/v1/ping");
}

std::string CURLRequest::getTime()
{
	return request("https://api.binance.com/api/v1/time");
}

std::string CURLRequest::getMarketDepth(const std::string &pair, unsigned int depth)
{
	return request("https://api.binance.com/api/v1/depth?symbol=LTCBTC&limit=5");
}

std::string CURLRequest::getPairsPricesVolume()
{
	return request("");
}

std::string CURLRequest::getPairsPrices()
{
	return request("");
}

std::string CURLRequest::getProducts()
{
	return request("https://www.binance.com/exchange/public/product");
}

std::string CURLRequest::getHistoricPrices(const std::string &symbol, const std::string &interval, unsigned int amount)
{
	std::stringstream ss;
	ss << "https://www.binance.com/api/v1/klines?symbol=";
	ss << symbol;
	ss << "&interval=" << interval;
	ss << "&limit=" << amount;

	return request(ss.str());
}

std::string CURLRequest::getCurrentPrices()
{
	return request("https://www.binance.com/api/v1/ticker/allPrices");
}

std::string CURLRequest::getAccountInformation()
{
	return request("https://www.binance.com/api/v3/account");
}

size_t CURLRequest::writeMemoryCallbackStatic(void *contents, size_t size, size_t nmemb, void *userp)
{
	CURLRequest *pObject = (CURLRequest*)userp;

	pObject->writeMemoryCallback(contents, size, nmemb);

	return size * nmemb;
}

void CURLRequest::writeMemoryCallback(void *contents, size_t size, size_t nmemb)
{
	size_t realsize = size * nmemb;

	// Check to make sure we haven't grown too big for the buffer
	if (m_BufferSize < (m_BufferOffset + realsize))
	{
		m_BufferSize = (m_BufferOffset + realsize) * 2;

		char *newBuffer = new char[m_BufferSize];
		memcpy(newBuffer, m_pBuffer, m_BufferOffset);
		delete m_pBuffer;
		m_pBuffer = newBuffer;
	}
	// Copy data to our buffer
	memcpy(m_pBuffer + m_BufferOffset, contents, realsize);
	m_BufferOffset += realsize;

	return;
}

std::string CURLRequest::request(const std::string &url)
{
//	curl_easy_setopt(m_pCurlHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEFUNCTION, CURLRequest::writeMemoryCallbackStatic);
	curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEDATA, (void *)this);
	m_BufferOffset = 0;

	CURLcode res = curl_easy_perform(m_pCurlHandle);

	if (res != CURLE_OK)
	{
		return "";
	}

	// NULL terminate
	m_pBuffer[m_BufferOffset] = 0;
	return std::string(m_pBuffer);
}