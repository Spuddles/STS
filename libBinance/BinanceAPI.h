#pragma once
#include <cassert>
#include "IRequests.h"
#include "Price.h"
#include "Product.h"
#include "AccountInfo.h"
#include "json.hpp"

using json = nlohmann::json;

enum INTERVAL
{
	ONEMIN,
	THREEMINS,
	FIVEMINS,
	FIFTEENMINS,
	THIRTYMINS,
	ONEHOUR,
	TWOHOURS,
	FOURHOURS,
	SIXHOURS,
	EIGHTHOURS,
	TWELVEHOURS,
	ONEDAY,
	THREEDAYS,
	ONEWEEK,
	ONEMONTH
};

/*std::string getInterval(INTERVAL interval)
{
	switch (interval)
	{
	case ONEMIN:		return "1m";
	case THREEMINS:		return "3m";
	case FIVEMINS:		return "5m";
	case FIFTEENMINS:	return "15m";
	case THIRTYMINS:	return "30m";
	case ONEHOUR:		return "1h";
	case TWOHOURS:		return "2h";
	case FOURHOURS:		return "4h";
	case SIXHOURS:		return "6h";
	case EIGHTHOURS:	return "8h";
	case TWELVEHOURS:	return "12h";
	case ONEDAY:		return "1d";
	case THREEDAYS:		return "3d";
	case ONEWEEK:		return "1w";
	case ONEMONTH:		return "1M";
	default:			assert(false);
	}
}*/

class BinanceAPI
{
public:
				BinanceAPI(IRequests *pRequests);
	virtual		~BinanceAPI();
	void		setKeys(const std::string &api, const std::string &secret);

	// UTILITY CALLS
	bool		getPing();
	bool		getTime(uint64_t &timestamp);

	// PRODUCTS
	bool		getProducts(std::vector<Product> &products);

	// PRICES
	bool		getHistoricPrices(std::string product, std::string interval, int amount, std::vector<Price> &vecPrices);
	bool		getCurrentPrices(std::vector<std::pair<std::string, double>> &vecPricePairs);

	// ACCOUNT DETAILS
	bool		getAccountInformation(AccountInfo &ai);

	// USER STREAMS
	bool		getListenKey(std::string &listenKey);
	bool		keepAliveListenKey(const std::string &key);
	bool		closeListeningKey(const std::string &key);

	// TRADES
	bool		sendNewOrder();
	bool		sendNewTestOrder();
	bool		cancelOrder();

private:
	// Binance is not consistent with double values in json,
	// some are stored as numerics while others are strings.
	// These methods handles both cases
	double		getDouble(const json &j, const std::string &field);
	double		getDouble(const json &j, unsigned int offset);

	IRequests  *m_pRequests;

	std::string	m_apiKey;
	std::string	m_secretKey;
};

