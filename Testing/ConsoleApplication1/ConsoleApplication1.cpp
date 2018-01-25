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

int main()
{
	// Create a database object
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open the database" << std::endl;
	}

	// Query Binance for list of products
	FileRequest fr("C:\\Users\\Spud\\OneDrive\\Trading\\Binance");
	WebSocketRequest wsr;

	BinanceAPI bapi(&wsr);

	std::vector<Product> vecProds;
	bapi.getProducts(vecProds);

	ProductCollection products(&db);

	for (Product p : vecProds)
	{
		if (!products.doesSymbolExist(p.getSymbol()))
		{
			products.addToDatabase(p);
		}
	}

    return 0;
}

