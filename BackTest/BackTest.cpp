#include "DataBase.h"
#include "CrumbleStrategy.h"
#include "json.hpp"
#include "Portfolio.h"
#include "Exchange.h"
#include "Helpers.h"
#include "Logger.h"
#include <iostream>
#include "WeightedMovingAverage.h"

using namespace STS;

using json = nlohmann::json;

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

void CheckForLiveTrading(const Price &price)
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

int main(int argc, char** argv)
{
	handleOptions(argc, argv);

	// Pull in the config options from the external file
	std::string config = Helpers::loadFile(g_options.config);
	json j = json::parse(config);
	std::string symbol = j["symbol"];
	double		orderSize = j["orderSize"];
	double		initialBTC = j["initialBTC"];
	int			minOrderGap = j["minOrderGap"];
	double		tradeTrigger = j["tradeTrigger"];

	// Create a log file
	Logger::setDefaultLog(j["logFile"]);

	DataBase db;
	if (!db.connect(g_options.database))
	{
		Log(CRITICAL, "Failed to open database");
		return 0;
	}

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

	// Load the details specific to this product
	Product	prod;
	db.getProduct(symbol, prod);

	// Create a portfolio and add some cash to it
	Portfolio portfolio;
	portfolio.initialiseFunds(prod);
	portfolio.updatePrice(vecPrices[0]);
	portfolio.addFunds(btcCoinID, initialBTC);

	// Create a basic fill-if-possible exchange
	Exchange  exchange(portfolio);

	// Create the strategy to run for each product
	CrumbleStrategy	cs(tradeTrigger, minOrderGap);

	double startPrice = vecPrices[0].getClose();
	std::stringstream ss;
	ss << "Total BTC value = " << portfolio.getBTCValue() << " at " << startPrice;
	Log(POSITION, ss.str());

	int count = 0;
	WeightedMovingAverage wma(0.15);

	for (Price price : vecPrices)
	{
		double	buyPrice = 0.0;
		double	sellPrice = 0.0;

		// Check to see if we have a continous price stream
		CheckForLiveTrading(price);

		portfolio.updatePrice(price);
		exchange.updatePrice(price.getProductID(), price);
		cs.updatePrice(price);

		if (g_bCanTrade && cs.isBuySignal())
		{
			buyPrice = price.getClose();
			if (exchange.getLiveOrderCount(price.getProductID()) < 2 &&
				portfolio.canAfford(prod, orderSize, buyPrice))
			{
				exchange.placeLimitOrder(prod, orderSize, price);
				cs.bought(price);
				g_buyTradesCount++;
			}
			else
			{
				// We can't buy anything, force a reset
				cs.resetBuySell();
			}
		}

		if (g_bCanTrade && cs.isSellSignal())
		{
			sellPrice = price.getClose();
			if (exchange.getLiveOrderCount(price.getProductID()) < 2)
			{
				double amount = 0.0 - portfolio.getAmount(prod.getBaseCoin());
				if (amount < 0.0)
				{
					exchange.placeLimitOrder(prod, amount, price);
					cs.sold(price);
					g_sellTradesCount++;
				}
				else
				{
					// We can sell anything so let algo now we want a buy
					cs.resetBuySell();
				}
			}
		}
		count++;
		ss.str("");
		wma.updatePrice(portfolio.getBTCValue());
		ss << "BTC, " << count << ", " << wma.getAverage();
		Log(POSITION, ss.str());
	}

	double endPrice = vecPrices[vecPrices.size()-1].getClose();

	std::stringstream pos;
	pos << "Total BTC value = " << portfolio.getBTCValue() << " at " << endPrice;
	Log(POSITION, pos.str());
	
	pos.str("");
	pos << "Sells:" << g_sellTradesCount << " Buys:" << g_buyTradesCount;
	Log(POSITION, pos.str());
    return 0;
}

