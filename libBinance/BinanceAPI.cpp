#include "BinanceAPI.h"
#include "sha256.h"
#include "hmac.h"

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

bool BinanceAPI::getHistoricPrices(std::string product, std::string interval, int amount, std::vector<Price> &vecPrices)
{
	std::string str = m_pRequests->getHistoricPrices(product, interval, amount);

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);

		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			Price price;
			json &p = (*it);

			price.setOpenTime(p[0]);
			price.setOpen(getDouble(p, 1));
			price.setHigh(getDouble(p, 2));
			price.setLow(getDouble(p, 3));
			price.setClose(getDouble(p, 4));
			price.setVolume(getDouble(p, 5));
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

			p.setMinQty(getDouble(i, "minQty"));
			p.setMinTrade(getDouble(i, "minTrade"));

			p.setQuoteAsset(i["quoteAsset"]);
			p.setQuoteAssetName(i["quoteAssetName"]);
			p.setQuoteAssetUnit(i["quoteAssetUnit"]);
			p.setStatus(i["status"]);
			p.setSymbol(i["symbol"]);

			p.setTickSize(getDouble(i, "tickSize"));
			p.setWithdrawFee(getDouble(i, "withdrawFee"));

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

bool BinanceAPI::getCurrentPrices(std::vector<std::pair<std::string, double>> &vecPricePairs)
{
	std::string str = m_pRequests->getCurrentPrices();

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);

		for (json::iterator it = j.begin(); it != j.end(); ++it)
		{
			json &p = (*it);

			std::cout << p.dump() << std::endl;

			std::string symbol = p["symbol"];
			double price = getDouble(p, "price");

			vecPricePairs.push_back(std::pair<std::string, double>(symbol, price));
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool BinanceAPI::getAccountInformation()
{
	// Build up the request
	std::stringstream ss;
	ss << "https://www.binance.com/api/v1/account";
	ss << "key=" << m_apiKey;
	ss << "signature=" << m_apiKey;

	std::string result = hmac<SHA256>(ss.str(), m_apiKey);

	m_pRequests->getRequest(ss.str());
	return false;
}

std::string	BinanceAPI::getListenKey()
{
	return "";
}

std::string BinanceAPI::keepAliveListenKey(const std::string & /*key*/)
{
	return "";
}

double BinanceAPI::getDouble(const json &j, const std::string &field)
{
	if (j[field].is_string())
	{
		std::string str = j[field];
		return atof(str.c_str());
	}
	return j[field];
}

double BinanceAPI::getDouble(const json &j, unsigned int offset)
{
	if (j[offset].is_string())
	{
		std::string str = j[offset];
		return atof(str.c_str());
	}
	return j[offset];
}

void BinanceAPI::setKeys(const std::string &api, const std::string &secret)
{
	m_apiKey = api;
	m_secretKey = secret;
}
