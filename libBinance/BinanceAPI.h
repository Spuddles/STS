#pragma once
#include <cassert>
#include "IRequests.h"
#include "Price.h"
#include "Product.h"

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

	bool		getPing();
	bool		getTime(uint64_t &timestamp);
	std::string	convertTime(uint64_t &timestamp);
	bool		getProducts(std::vector<Product> &products);
	bool		getPrices(std::string product, std::string interval, int amount, std::vector<Price> &vecPrices);

private:
	IRequests  *m_pRequests;
};

