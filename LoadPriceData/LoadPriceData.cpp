// LoadPriceData.cpp : Defines the entry point for the console application.
//

#include "DataBase.h"
#include "MovingAverage.h"
#include "WeightedMovingAverage.h"
#include "BollingerBand.h"
#include "Variance.h"

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

	std::vector<Price>	vecPrices;
	db.getPriceData(BNBBTC_PAIR, vecPrices);

	Product bnbbtc;
	db.getProduct(BNBBTC_PAIR, bnbbtc);

	Portfolio portfolio;
	portfolio.addFunds(BNB_COIN, 100);
	portfolio.addFunds(BTC_COIN, 1);

	Exchange  exchange(portfolio);

	int count = 0;
	for (Price p : vecPrices)
	{
		portfolio.updatePrice(BNBBTC_PAIR, p);
		exchange.updatePrice(BNBBTC_PAIR, p);

		if (count % 10 == 0)
		{
			// Add a new order
			if (exchange.getLiveOrderCount(BNBBTC_PAIR) == 0)
			{
				double amount = (std::rand() % 20) - 10;
				exchange.placeLimitOrder(bnbbtc, amount, p.getClose());
			}
		}
		count++;
	}

	return 0;

/*	MovingAverage ma(10);
	WeightedMovingAverage wma(0.2);
	BollingerBand bb(21, 2.0);

	for (Price p : vecPrices)
	{
		double maValue = ma.addPrice(p.getClose());
		double wmaValue = wma.addPrice(p.getClose());
		double bbValue = bb.addPrice(p.getClose());

		std::cout << p.getClose() << "," << bbValue << ",";
		std::cout << bb.getLowerValue() << "," << bb.getUpperValue() << std::endl;

//		std::cout << p.getMid() << "," << maValue << "," << wmaValue;
//		std::cout << "," << bb.getLowerValue() << "," << bb.getUpperValue() << std::endl;
	}*/

    return 0;
}

