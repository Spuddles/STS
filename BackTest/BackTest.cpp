#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <algorithm>

#include "DataBase.h"
#include "IStrategy.h"
#include "Exchange.h"
#include "Helpers.h"
#include "Logger.h"
#include "WeightedMovingAverage.h"
#include "Params.h"
#include "DynamicLoader.h"
#include "AccountInfo.h"

using namespace STS;

int			g_buyTradesCount = 0;
int			g_sellTradesCount = 0;
uint64_t	g_previousPriceTime = 0;
uint8_t		g_intervalsBeforeLive = 30;
bool		g_bCanTrade = false;

struct options
{
	std::string database = "Trading.sqlite";
	std::string config = "Crumble.json";
	bool		writeTrades = false;
	bool		writePosition = false;
} g_options;

void handleOptions(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::string str = argv[i];
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);

		if (str.compare("-DB") == 0)
		{
			if (i < argc - 1)
			{
				++i;
				g_options.database = argv[i];
			}
		}
		
		if (str.compare("-CONFIG") == 0)
		{
			if (i < argc - 1)
			{
				++i;
				g_options.config = argv[i];
			}
		}

		if (str.compare("-WT") == 0)
		{
			g_options.writeTrades = true;
		}
	}
}

void checkForLiveTrading(const Price &price)
{
	if (price.getOpenTime() > (g_previousPriceTime + 60000) && g_previousPriceTime > 0)
	{
		g_bCanTrade = false;
		g_intervalsBeforeLive = 30;
		std::stringstream ss;
		ss << "We have non contiguous data from ";
		ss << Helpers::convertTime(g_previousPriceTime) << " to ";
		ss << Helpers::convertTime(price.getOpenTime());
		ss << " (" << ((price.getOpenTime() - g_previousPriceTime) / 60000LL) << " minutes)";
		Log(DEBUG, ss.str());
	}
	g_previousPriceTime = price.getOpenTime();
	if (!g_bCanTrade)
	{
		if (--g_intervalsBeforeLive == 0)
		{
			g_bCanTrade = true;
		}
	}
}

void getStartAndEndDates(const Params &p, uint64_t &startTS, uint64_t &endTS)
{
	std::string startDate;
	std::string endDate;

	if (p.getString("startDate", startDate))
	{
		startTS = Helpers::convertDateToTimestamp(startDate);
	}
	else
	{
		startTS = 0;
	}

	if (p.getString("endDate", endDate))
	{
		endTS = Helpers::convertDateToTimestamp(endDate);
	}
	else
	{
		endTS = Helpers::convertDateToTimestamp("2099-12-31");
	}
}

int main(int argc, char** argv)
{
	handleOptions(argc, argv);

	// Pull in the config options from the external file
	Params p;
	p.load(g_options.config);

	// Create a log file
	std::stringstream lf;
	std::string symbol;
	p.getString("symbol", symbol);
	std::string stratName;
	p.getString("strategyName", stratName);
	std::string logLevelStr;
	p.getString("logLevel", logLevelStr);

	lf << stratName << "-" << symbol << ".log";
	Logger::setDefaultLog(lf.str());
	Logger::setLogLevel(logLevelStr);

	DataBase db;
	if (!db.connect(g_options.database))
	{
		Log(CRITICAL, "Failed to open database");
		return 0;
	}

	// Load the details specific to this product
	Product	prod;
	db.getProduct(symbol, prod);

	// Get the historic sorted dump of 1 minute price intervals
	std::vector<Price>	vecPrices;
	db.getPriceData(symbol, vecPrices);

	// Find out BTC coin ID
	unsigned int btcCoinID;
	if (!db.getCoinID("BTC", btcCoinID))
	{
		Log(CRITICAL, "Failed to get BTC coin ID");
		return -1;
	}

	// Get the start and end time for the backtest
	uint64_t		startTS = 0LL;
	uint64_t		endTS = 0LL;
	getStartAndEndDates(p, startTS, endTS);

	double		initialBTC;
	int			minOrderGap;
	double		tradeTrigger;
	for (size_t i = 0; i < p.getRangeCount(); i++)
	{
		g_buyTradesCount = 0;
		g_sellTradesCount = 0;

		p.getDouble("initialBTC", initialBTC);
		p.getInt("minOrderGap", minOrderGap);
		p.getDouble("tradeTrigger", tradeTrigger);

		AccountInfo ai;
		ai.insertHolding("BTC", 0.1, 0.0);

		// Create a basic fill-if-possible exchange
		Exchange  exchange(ai);

		// Attempt to load the strategy
		IStrategy *cs = DynamicLoader::loadModule("Crumble.dll");
		if (!cs)
		{
			Log(CRITICAL, "Failed to load the strategy DLL");
			return 0;
		}
		cs->initialise(p);

		double startPrice = vecPrices[0].getClose();
		std::stringstream ss;
// TODO		ss << "Total BTC value = " << portfolio.getBTCValue() << " at " << startPrice;
		Log(POSITION, ss.str());

		int count = 0;
		WeightedMovingAverage wma(0.15);

		for (Price price : vecPrices)
		{
			if (price.getCloseTime() < startTS ||
				price.getCloseTime() > endTS)
			{
				continue;
			}

			double	buyPrice = 0.0;
			double	sellPrice = 0.0;

			// Check to see if we have a continous price stream
			checkForLiveTrading(price);

// TODO			portfolio.updatePrice(price);
			exchange.updatePrice(price.getProductID(), price);
			cs->updatePrice(price);

			if (g_bCanTrade && cs->isBuySignal())
			{
				buyPrice = price.getClose();
				if (exchange.getLiveOrderCount(price.getProductID()) < 2)
				{
					// Work out how much we can buy
					double amount = 1; // TODO portfolio.getAmount(prod.getQuoteCoin()) / buyPrice;
					double tradeSize = std::floor(amount*10.0) / 10.0;

					exchange.placeLimitOrder(prod, tradeSize, price);
					cs->bought(price);
					g_buyTradesCount++;
				}
				else
				{
					// We can't buy anything, force a reset
					cs->resetBuySell();
				}
			}

			if (g_bCanTrade && cs->isSellSignal())
			{
				sellPrice = price.getClose();
				if (exchange.getLiveOrderCount(price.getProductID()) < 2)
				{
					double amount = 0.0 - 1.0; // TODO  portfolio.getAmount(prod.getBaseCoin());
					if (amount < 0.0)
					{
						exchange.placeLimitOrder(prod, amount, price);
						cs->sold(price);
						g_sellTradesCount++;
					}
					else
					{
						// We can sell anything so let algo now we want a buy
						cs->resetBuySell();
					}
				}
			}
			count++;
			ss.str("");
// TODO			wma.updatePrice(portfolio.getBTCValue());
			ss << "BTC, " << count << ", " << wma.getAverage();
			Log(POSITION, ss.str());
		}

		double endPrice = vecPrices[vecPrices.size() - 1].getClose();

		std::stringstream pos;
// TODO		pos << "Total BTC value = " << portfolio.getBTCValue() << " at " << endPrice;
		Log(POSITION, pos.str());

		pos.str("");
		pos << "Sells:" << g_sellTradesCount << " Buys:" << g_buyTradesCount;
		Log(POSITION, pos.str());

		pos.str("");
		pos << std::setprecision(6);
		pos << symbol << ", ";
// TODO		pos << portfolio.getBTCValue() << ", ";
		pos << initialBTC << ", ";
		pos << minOrderGap << ", ";
		pos << tradeTrigger << ", ";
		pos << g_buyTradesCount << ", ";
		pos << g_sellTradesCount;

		Log(PNL, pos.str());

		// Output progress when we are doing more than one iteration
		if (p.getRangeCount() > 1)
		{
			std::cout << i + 1 << "/" << p.getRangeCount() << ":" << pos.str() << std::endl;
		}
		delete cs;
		p.next();
	}
    return 0;
}

