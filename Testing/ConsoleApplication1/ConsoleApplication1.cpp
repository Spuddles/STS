// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "json.hpp"
#include <fstream>
#include <streambuf>
#include <iostream>

#include "FileRequest.h"
#include "BinanceAPI.h"
#include "Product.h"
#include "ProductCollection.h"
#include "DataBase.h"
#include "WebSocketRequest.h"

using json = nlohmann::json;

void updateProducts(WebSocketRequest &wsr, DataBase &db)
{
	BinanceAPI bapi(&wsr);

	std::vector<Product> vecProds;
	bapi.getProducts(vecProds);

	for (Product p : vecProds)
	{
		bool exists;
		db.doesProductExist(p.getSymbol(), exists);
		if (exists)
		{
			int id;
			if (db.getProductID(p.getSymbol(), id))
			{
				p.setID(id);
				db.updateProduct(p);
			}
		}
		else
		{
			db.insertProduct(p);
		}
	}
}

void testFromFile(DataBase &db)
{
	FileRequest fr("C:\\Users\\Spud\\OneDrive\\Trading\\Binance");
	BinanceAPI bapi(&fr);
}

int main()
{
	// Create a database object
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open the database" << std::endl;
	}

	// Query Binance for list of products
	WebSocketRequest wsr;

	updateProducts(wsr, db);

	return 0;
	
	
	
	BinanceAPI bapi(&wsr);

	std::vector<Product>	vecProducts;
	db.getAllProducts(vecProducts);

	for (Product prod : vecProducts)
	{
		std::string symbol = prod.getSymbol();
		int			id = prod.getID();

		std::cout << "Getting prices for " << symbol << std::endl;

		std::vector<Price>	vecPrices;
		bapi.getPrices(symbol, "1m", 500, vecPrices);

		for (Price p : vecPrices)
		{
			db.insertPrice(id, p);
		}
	}
    return 0;
}

