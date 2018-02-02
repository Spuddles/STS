#pragma once
#include <string>
#include <map>
#include <vector>

class IRequests
{
public:
					IRequests() {};
	virtual			~IRequests() {};

	virtual std::string	ping() = 0;
	virtual std::string	getTime() = 0;
	virtual std::string	getMarketDepth(const std::string &pair, unsigned int depth) = 0;
	virtual std::string	getPairsPricesVolume() = 0;
	virtual std::string	getPairsPrices() = 0;
	virtual std::string	getProducts() = 0;
	virtual std::string getHistoricPrices(const std::string &symbol, const std::string &interval, unsigned int amount) = 0;
	virtual std::string getCurrentPrices() = 0;
};

