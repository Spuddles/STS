#include "stdafx.h"
#include "WebSocketRequest.h"
#include <iostream>
#include <sstream>

WebSocketRequest::WebSocketRequest()
{
	curl_global_init(CURL_GLOBAL_ALL);
	std::cout << "Curl version:" << curl_version() << std::endl;
	m_pCurlHandle = curl_easy_init();
}

WebSocketRequest::~WebSocketRequest()
{
	/* cleanup curl stuff */
	curl_easy_cleanup(m_pCurlHandle);

	/* we're done with libcurl, so clean it up */
	curl_global_cleanup();
}

std::string WebSocketRequest::ping()
{
	return request("https://api.binance.com/api/v1/ping");
}

std::string WebSocketRequest::getTime()
{
	return request("https://api.binance.com/api/v1/time");
}

std::string WebSocketRequest::getMarketDepth(std::string pair, unsigned int depth)
{
	return request("https://api.binance.com/api/v1/depth?symbol=LTCBTC&limit=5");
}

std::string WebSocketRequest::getPairsPricesVolume()
{
	return request("");
}

std::string WebSocketRequest::getPairsPrices()
{
	return request("");
}

std::string WebSocketRequest::getProducts()
{
	return request("https://www.binance.com/exchange/public/product");
}

std::string WebSocketRequest::getPrices(std::string symbol, std::string interval, unsigned int amount)
{
	std::stringstream ss;
	ss << "https://www.binance.com/api/v1/klines?symbol=";
	ss << symbol;
	ss << "&interval=" << interval;
	ss << "&limit=" << amount;

	return request(ss.str());
}

size_t WebSocketRequest::WriteMemoryCallbackStatic(void *contents, size_t size, size_t nmemb, void *userp)
{
	WebSocketRequest *pObject = (WebSocketRequest*)userp;

	pObject->WriteMemoryCallback(contents, size, nmemb);

	return size * nmemb;
}

void WebSocketRequest::WriteMemoryCallback(void *contents, size_t size, size_t nmemb)
{
	size_t realsize = size * nmemb;

	// Copy data to our buffer
	memcpy(m_Buffer + m_BufferOffset, contents, realsize);

	m_BufferOffset += realsize;

	return;
}

std::string WebSocketRequest::request(std::string url)
{
//	curl_easy_setopt(m_pCurlHandle, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEFUNCTION, WebSocketRequest::WriteMemoryCallbackStatic);
	curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEDATA, (void *)this);
	m_BufferOffset = 0;
	memset(m_Buffer, -1, 200 * 1024);

	CURLcode res = curl_easy_perform(m_pCurlHandle);

	/* check for errors */
	if (res != CURLE_OK)
	{
		return "";
	}

	// NULL terminate
	m_Buffer[m_BufferOffset] = 0;

	std::cout << "Size of buffer = " << m_BufferOffset << std::endl;

	return std::string(m_Buffer);
}