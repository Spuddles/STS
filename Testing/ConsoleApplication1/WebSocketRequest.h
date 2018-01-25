#pragma once
#include "IRequests.h"
#include "curl/curl.h"

class WebSocketRequest: public IRequests
{
public:
			WebSocketRequest();
	virtual ~WebSocketRequest();

	std::string	ping();
	std::string	getTime();
	std::string	getMarketDepth(std::string pair, unsigned int depth);
	std::string	getPairsPricesVolume();
	std::string	getPairsPrices();
	std::string	getProducts();
	std::string getPrices(std::string symbol, std::string interval, unsigned int amount);

	std::string request(std::string url);

	static	size_t WriteMemoryCallbackStatic(void *contents, size_t size, size_t nmemb, void *userp);
	void	WriteMemoryCallback(void *contents, size_t size, size_t nmemb);


private:

	CURL	*m_pCurlHandle;
	char	m_Buffer[200*1024];	//1M
	size_t	m_BufferOffset;
};

