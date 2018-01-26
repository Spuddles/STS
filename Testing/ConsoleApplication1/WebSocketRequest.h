#pragma once
#include "IRequests.h"
#include "curl/curl.h"

class WebSocketRequest: public IRequests
{
public:
				WebSocketRequest();
	virtual		~WebSocketRequest();

	std::string	ping();
	std::string	getTime();
	std::string	getMarketDepth(const std::string &pair, unsigned int depth);
	std::string	getPairsPricesVolume();
	std::string	getPairsPrices();
	std::string	getProducts();
	std::string getPrices(const std::string &symbol, const std::string &interval, unsigned int amount);

	std::string request(const std::string &url);

	static	size_t	writeMemoryCallbackStatic(void *contents, size_t size, size_t nmemb, void *userp);
	void			writeMemoryCallback(void *contents, size_t size, size_t nmemb);

private:
	CURL	*m_pCurlHandle;
	char	*m_pBuffer;
	size_t	m_BufferSize;
	size_t	m_BufferOffset;
};

