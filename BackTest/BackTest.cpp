#include "DataBase.h"
#include "CrumbleStrategy.h"
#include "json.hpp"
#include "Portfolio.h"
#include "Exchange.h"
#include "Helpers.h"

#include <iostream>

#define PORTFOLIO_INFO

using json = nlohmann::json;

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
	//std::cout << "Using DB '" << g_options.database << "' and CONFIG '";
	//std::cout << g_options.config << "'" << std::endl;
}

int main(int argc, char** argv)
{
	handleOptions(argc, argv);

	std::string config = Helpers::loadFile(g_options.config);
	json j = json::parse(config);

	DataBase db;
	// TODO HERE Check DB file exists so we don't create a new empty one
	// Needs a new exists function in Helpers.cpp
	if (!db.connect(g_options.database))
	{
		std::cout << "Failed to open database" << std::endl;
		return 0;
	}

	// Get the historic sorted dump of 1 minute price intervals
	std::string symbol = j["symbol"];
	int			orderSize = j["orderSize"];
	int			initialBTC = j["initialBTC"];
	int			minOrderGap = j["minOrderGap"];

	std::vector<Price>	vecPrices;
	db.getPriceData(symbol, vecPrices);

	// Find out BTC coin ID
	unsigned int btcCoinID;
	if (!db.getCoinID("BTC", btcCoinID))
	{
		std::cout << "Failed to get BTC coin ID" << std::endl;
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

	// Create a basic fill if possible exchange
	Exchange  exchange(portfolio);

	// Create the strategy to run for each product
	CrumbleStrategy	cs;

	if (g_options.writePosition)
	{
		std::cout << "Total BTC value = " << portfolio.getBTCValue() << std::endl;
	}

	int count = 0;
	int buyTrades = 0;
	int sellTrades = 0;
	for (Price price : vecPrices)
	{
		double	buyPrice = 0.0;
		double	sellPrice = 0.0;

		portfolio.updatePrice(price);
		exchange.updatePrice(price.getProductID(), price);
		cs.updatePrice(price);

		if (exchange.minutesSinceLastTrade(price.getCloseTime()) > minOrderGap)
		{
			if (cs.isBuySignal())
			{
				buyPrice = price.getClose();
				if (exchange.getLiveOrderCount(price.getProductID()) == 0 &&
					portfolio.canAfford(prod, orderSize, price.getClose()))
				{
					exchange.placeLimitOrder(prod, orderSize, price);
					buyTrades++;
				}
			}

			if (cs.isSellSignal())
			{
				sellPrice = price.getClose();
				if (exchange.getLiveOrderCount(price.getProductID()) == 0)
				{
					double amount = 0.0 - portfolio.getAmount(prod.getBaseCoin());
					if (amount < 0.0)
					{
						exchange.placeLimitOrder(prod, amount, price);
						sellTrades++;
					}
				}
			}
			count++;
		}

		if (g_options.writeTrades)
		{
			std::cout << price.getClose() << ",";
			if (buyPrice > 0.0)
			{
				std::cout << buyPrice;
			}
			std::cout << ",";
			if (sellPrice > 0.0)
			{
				std::cout << sellPrice;
			}
			std::cout << "," << portfolio.getBTCValue();
			std::cout << std::endl;
		}
	}
	if (g_options.writePosition)
	{
		std::cout << "Total BTC value = " << portfolio.getBTCValue() << std::endl;
		std::cout << "Sells:" << sellTrades << " Buys:" << buyTrades << std::endl;
	}
    return 0;
}

