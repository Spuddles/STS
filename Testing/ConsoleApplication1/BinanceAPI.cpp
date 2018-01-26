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

bool BinanceAPI::getPrices(std::string product, std::string interval, int amount, std::vector<Price> &vecPrices)
{
	std::string str = m_pRequests->getPrices(product, interval, amount);

	if (str.empty())
		return false;

	//std::cout << str << std::endl;

	try
	{
		json j = json::parse(str);

		size_t items = j.size();

		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			Price price;
			json &p = (*it);

			price.setOpenTime(p[0]);
			std::string openstr = p[1];
			price.setOpen(atof(openstr.c_str()));
			std::string highstr = p[2];
			price.setHigh(atof(highstr.c_str()));
			std::string lowstr = p[3];
			price.setLow(atof(lowstr.c_str()));
			std::string closestr = p[4];
			price.setClose(atof(closestr.c_str()));
			std::string volumestr = p[5];
			price.setVolume(atof(volumestr.c_str()));
			price.setCloseTime(p[6]);
			price.setTrades(p[8]);

			vecPrices.push_back(price);
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
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

		json &i = (*it);

		try
		{
			p.setActive(i["active"]);
			p.setBaseAsset(i["baseAsset"]);
			p.setBaseAssetName(i["baseAssetName"]);
			p.setBaseAssetUnit(i["baseAssetUnit"]);

			p.setDecimalPlaces(i["decimalPlaces"]);

			p.setMatchingUnitType(i["matchingUnitType"]);

			std::string minQty = i["minQty"];
			p.setMinQty(atof(minQty.c_str()));
			std::string minTrade = i["minTrade"];
			p.setMinTrade(atof(minTrade.c_str()));

			p.setQuoteAsset(i["quoteAsset"]);
			p.setQuoteAssetName(i["quoteAssetName"]);
			p.setQuoteAssetUnit(i["quoteAssetUnit"]);
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
