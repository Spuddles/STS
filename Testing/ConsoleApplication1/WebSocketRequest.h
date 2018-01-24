#pragma once
#include "IRequests.h"

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
};

