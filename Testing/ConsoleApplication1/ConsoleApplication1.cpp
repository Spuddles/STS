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

using json = nlohmann::json;

int main()
{
	// Create a database object
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open the database" << std::endl;
	}

	Product p;
	p.setActive(true);
	p.setBaseAsset("BTC");
	p.setBaseAssetName("Bitcoin");
	p.setBaseAssetUnit("");

	db.insertProduct(p);

	std::vector<Product> vecProducts;
	db.getAllProducts(vecProducts);

	return 0;


	// Create the products object
	ProductCollection products;
	products.loadFromDatabase(nullptr);

	// Query Binance for list of products
	FileRequest fr("C:\\Users\\Spud\\OneDrive\\Trading\\Binance");
	BinanceAPI bapi(&fr);

	std::vector<Product> vecProds;
	bapi.getProducts(vecProds);

	for (Product p : vecProds)
	{
		if (!products.doesSymbolExist(p.getSymbol()))
		{
			products.addToDatabase(p);
		}
	}

    return 0;
}

