// LoadPriceData.cpp : Defines the entry point for the console application.
//

#include "DataBase.h"
#include "MovingAverage.h"
#include "WeightedMovingAverage.h"
#include "BollingerBand.h"
#include "Variance.h"
#include <iostream>

int main()
{
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open database" << std::endl;
		return 0;
	}

	std::vector<Price>	vecPrices;
	db.getPriceData(3, vecPrices);

	MovingAverage ma(10);
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
	}

    return 0;
}

