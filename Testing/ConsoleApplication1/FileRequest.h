#pragma once
#include <string>
#include "IRequests.h"

class FileRequest : public IRequests
{
public:
				FileRequest(const std::string &path);
	virtual		~FileRequest();

	std::string	ping();
	std::string	getTime();
	std::string	getMarketDepth(const std::string &pair, unsigned int depth);
	std::string	getPairsPricesVolume();
	std::string	getPairsPrices();
	std::string	getProducts();
	std::string getPrices(const std::string &symbol, const std::string &interval, unsigned int amount);

private:
	std::string m_Path;

	std::string loadFile(const std::string &filename);
};

