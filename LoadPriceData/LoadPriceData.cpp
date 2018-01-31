// LoadPriceData.cpp : Defines the entry point for the console application.
//

#include "DataBase.h"
#include "MovingAverage.h"
#include "WeightedMovingAverage.h"
#include "BollingerBand.h"
#include "Variance.h"
#include "CrumbleStrategy.h"

#include "Portfolio.h"
#include "Exchange.h"

#include <iostream>
#include <random>

#define	BNBBTC_PAIR	(3)
#define	BNB_COIN	(2)
#define BTC_COIN	(4)

int main()
{
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open database" << std::endl;
		return 0;
	}

	// Get the historic sorted dump of 1 minute price intervals
	std::vector<Price>	vecPrices;
	db.getPriceData(BNBBTC_PAIR, vecPrices);

	// Load the details specific to BNBBTC
	Product bnbbtc;
	db.getProduct(BNBBTC_PAIR, bnbbtc);

	// Create a portfolio and add some cash to it
	Portfolio portfolio;
	portfolio.addFunds(BNB_COIN, 0);
	portfolio.addFunds(BTC_COIN, 0.2);

	// Create a basic fill if possible exchange
	Exchange  exchange(portfolio);

	// Create the strategy to run
	CrumbleStrategy cs;

//	portfolio.displayPosition();
	int count = 0;
	int buyTrades = 0;
	int sellTrades = 0;
	for (Price p : vecPrices)
	{
		double	buyPrice = 0.0;
		double	sellPrice = 0.0;

		exchange.updatePrice(BNBBTC_PAIR, p);
		cs.updatePrice(p);

		if (cs.isBuySignal())
		{
			buyPrice = p.getClose();
			if (exchange.getLiveOrderCount(BNBBTC_PAIR) == 0 &&
				portfolio.canAfford(bnbbtc, 100, p.getClose()))
			{
				exchange.placeLimitOrder(bnbbtc, 100, p.getClose());
				buyTrades++;
			}
		}

		if (cs.isSellSignal())
		{
			sellPrice = p.getClose();
			if (exchange.getLiveOrderCount(BNBBTC_PAIR) == 0)
			{
				double amount = 0.0-portfolio.getAmount(BNB_COIN);
				if (amount < 0.0)
				{
					exchange.placeLimitOrder(bnbbtc, amount, p.getClose());
					sellTrades++;
				}
			}
		}
		count++;
		std::cout << p.getClose() << ",";
		if (buyPrice > 0.0)
		{
			std::cout << buyPrice;
		}
		std::cout << ",";
		if (sellPrice > 0.0)
		{
			std::cout << sellPrice;
		}
		std::cout << "," << portfolio.getUSDValue();
		std::cout << std::endl;
	}
//	portfolio.displayPosition();
    return 0;
}

