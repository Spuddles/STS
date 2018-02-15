#include "BinanceAPI.h"
#include "sha256.h"
#include "hmac.h"
#include "Helpers.h"

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

bool BinanceAPI::getAccountInformation(AccountInfo &ai)
{
	// Build up the request
	std::string url = "https://www.binance.com/api/v3/account?";

	std::stringstream ss;
	ss << "timestamp=";
	ss << Helpers::getCurrentTimestamp();
	std::string signature = hmac<SHA256>(ss.str(), m_secretKey);
	ss << "&signature=" << signature;

	std::string str = m_pRequests->getRequest(url + ss.str(), "GET", m_apiKey);

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);

		ai.setCanTrade(j["canTrade"]);
		ai.setCanWithdraw(j["canWithdraw"]);
		ai.setCanDeposit(j["canDeposit"]);

		json b = j["balances"];

		for (json::iterator it = b.begin(); it != b.end(); ++it)
		{
			json &h = (*it);

			std::cout << h.dump() << std::endl;

			std::string coin = h["asset"];
			double free = getDouble(h, "free");
			double locked = getDouble(h, "locked");

			ai.insertHolding(coin, free, locked);
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool BinanceAPI::getListenKey(std::string &listenKey)
{
	std::string url = "https://www.binance.com/api/v1/userDataStream";

	std::string str = m_pRequests->getRequest(url, "POST", m_apiKey);

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);

		listenKey = j["listenKey"];
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool BinanceAPI::keepAliveListenKey(const std::string &key)
{
	std::stringstream ss;
	
	ss << "https://www.binance.com/api/v1/userDataStream?";
	ss << "listenKey=" << key;

	std::string str = m_pRequests->getRequest(ss.str(), "PUT", m_apiKey);

	return (!str.empty() && (str.find("{}") != std::string::npos) );
}

bool BinanceAPI::closeListeningKey(const std::string &key)
{
	std::stringstream ss;

	ss << "https://www.binance.com/api/v1/userDataStream?";
	ss << "listenKey=" << key;

	std::string str = m_pRequests->getRequest(ss.str(), "DELETE", m_apiKey);

	return (!str.empty() && (str.find("{}") != std::string::npos));
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

bool BinanceAPI::sendNewLimitOrder(const std::string &symbol,
									eSide side,
									eType type,
									eTimeInForce timeInForce,
									double quantity,
									double price)
{
	std::string url = "https://www.binance.com/api/v3/order?";

	std::stringstream ss;
	ss << "symbol=" << symbol;
	ss << "&side=" << getSideStr(side);
	ss << "&type=" << getTypeStr(type);
	ss << "&timeInForce=" << getTimeInForceStr(timeInForce);
	ss << "&quantity=" << quantity;
	ss << "&price=" << price;

	// I only care that the order has been accepted as I hope to get
	// the execute results on the user stream
	ss << "&newOrderRespType=ACK";
	ss << "&timestamp=" << Helpers::getCurrentTimestamp();
	std::string signature = hmac<SHA256>(ss.str(), m_secretKey);
	ss << "&signature=" << signature;

	std::string str = m_pRequests->getRequest(url + ss.str(), "POST", m_apiKey);

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool BinanceAPI::sendNewLimitTestOrder(const std::string &symbol,
	eSide side,
	eType type,
	eTimeInForce timeInForce,
	double quantity,
	double price)
{
	std::string url = "https://www.binance.com/api/v3/order/test?";

	std::stringstream ss;
	ss << "symbol=" << symbol;
	ss << "&side=" << getSideStr(side);
	ss << "&type=" << getTypeStr(type);
	ss << "&timeInForce=" << getTimeInForceStr(timeInForce);
	ss << "&quantity=" << quantity;
	ss << "&price=" << price;
	ss << "&timestamp=" << Helpers::getCurrentTimestamp();
	std::string signature = hmac<SHA256>(ss.str(), m_secretKey);
	ss << "&signature=" << signature;

	std::string str = m_pRequests->getRequest(url + ss.str(), "POST", m_apiKey);

	if (str.empty())
		return false;

	try
	{
		json j = json::parse(str);
	}
	catch (std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return false;
	}
	return true;
}

bool BinanceAPI::cancelOrder(const std::string &symbol, const std::string &orderID)
{
	std::string url = "https://www.binance.com/api/v3/order?";

	std::stringstream ss;

	return false;
}

std::string BinanceAPI::getSideStr(eSide side) const
{
	switch (side)
	{
	case BUY:	return "BUY";
	case SELL:	return "SELL";
	default:	assert(false);
	}
}

std::string BinanceAPI::getTypeStr(eType type) const
{
	switch (type)
	{
	case LIMIT:				return "LIMIT";
	case MARKET:			return "MARKET";
	case STOP_LOSS:			return "STOP_LOSS";
	case STOP_LOSS_LIMIT:	return "STOP_LOSS_LIMIT";
	case TAKE_PROFIT:		return "TAKE_PROFIT";
	case TAKE_PROFIT_LIMIT:	return "TAKE_PROFIT_LIMIT";
	case LIMIT_MAKER:		return "LIMIT_MAKER";
	default: assert(false);
	}
}

std::string BinanceAPI::getTimeInForceStr(eTimeInForce tif) const
{
	switch (tif)
	{
		case GTC:	return "GTC";
		case IOC:	return "IOC";
		case FOK:	return "FOK";
		default:	assert(false);
	}
}
