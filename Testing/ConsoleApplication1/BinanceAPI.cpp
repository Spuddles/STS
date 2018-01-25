#include "stdafx.h"
#include "BinanceAPI.h"
#include "json.hpp"

#include <iostream>
#include <time.h>

using json = nlohmann::json;

BinanceAPI::BinanceAPI(IRequests *pRequests) :
	m_pRequests(pRequests)
{
}

BinanceAPI::~BinanceAPI()
{
}

bool BinanceAPI::getPing()
{
	std::string str = m_pRequests->ping();

	return (str.compare("{}")==0);
}

bool BinanceAPI::getTime(uint64_t &timestamp)
{
	std::string str = m_pRequests->getTime();

	if (str.empty())
		return false;

	std::cout << str << std::endl;

	json j = json::parse(str);

	try
	{
		timestamp = j["serverTime"];
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
		return false;
	}

	return true;
}

std::string BinanceAPI::convertTime(uint64_t &timestamp)
{
	int milliseconds = timestamp % 1000;
	time_t tt = timestamp / 1000;
	struct tm ts;
	localtime_s(&ts, &tt);

	char buffer[64];
	sprintf_s(buffer, 64, "%04d%02d%02d-%02d:%02d:%02d.%04d", ts.tm_year + 1900, ts.tm_mon+1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, milliseconds);

	return std::string(buffer);
}

bool BinanceAPI::getPrices(std::vector<Price> &prices)
{
	std::string symbol = "LTCBTC";
	std::string interval = "1m";
	unsigned int amount = 500;
	std::string str = m_pRequests->getPrices(symbol, interval, amount);

	return false;
}

bool BinanceAPI::getProducts(std::vector<Product> &products)
{
	std::string str = m_pRequests->getProducts();

	if (str.empty())
		return false;

	json j = json::parse(str);

	size_t items = j.size();

	json jj = j["data"];

	for (json::iterator it = jj.begin();it != jj.end();++it)
	{
		Product p;
		//std::cout << it->dump(4) << std::endl;

		json &i = (*it);

		try
		{
			p.setActive(i["active"]);
			p.setBaseAsset(i["baseAsset"]);
			p.setDecimalPlaces(i["decimalPlaces"]);

			std::string minQty = i["minQty"];
			p.setMinQty(atof(minQty.c_str()));

			std::string minTrade = i["minTrade"];
			p.setMinTrade(atof(minTrade.c_str()));
			p.setQuoteAsset(i["quoteAsset"]);
			p.setStatus(i["status"]);

			p.setSymbol(i["symbol"]);

			std::string tickSize = (*it)["tickSize"];
			p.setTickSize(atof(tickSize.c_str()));

			std::string withdrawFee = (*it)["withdrawFee"];
			p.setWithdrawFee(atof(withdrawFee.c_str()));

			products.push_back(p);
		}
		catch (std::exception ex)
		{
			std::cout << ex.what() << std::endl;
			std::cout << "While processing the block:" << std::endl;
			std::cout << it->dump(4) << std::endl;
		}
	}

	return true;
}
