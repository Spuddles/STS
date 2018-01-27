#include <fstream>
#include <streambuf>
#include <iostream>

#include "BinanceAPI.h"
#include "Product.h"
#include "DataBase.h"
#include "WebSocketRequest.h"

void updateProducts(WebSocketRequest &wsr, DataBase &db)
{
	BinanceAPI bapi(&wsr);

	// Set all the products as inactive
	if (!db.clearActiveFlag())
	{
		std::cout << "Failed to clear active flags" << std::endl;
	}

	// Get all the products from binance
	std::vector<Product> vecProds;
	bapi.getProducts(vecProds);
	std::cout << "Received " << vecProds.size() << " products from binance" << std::endl;

	// Insert or update them in the DB
	unsigned int updated=0, inserted=0;
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
				updated++;
			}
		}
		else
		{
			db.insertProduct(p);
			inserted++;
		}
	}
	std::cout << "Inserted " << inserted << " and updated ";
	std::cout << updated << " products." << std::endl;
}

void getPrices(WebSocketRequest &wsr, DataBase &db)
{
	BinanceAPI bapi(&wsr);

	std::vector<Product>	vecProducts;
	db.getAllProducts(vecProducts);

	std::cout << "Collecting prices for " << vecProducts.size() << " active products" << std::endl;

	unsigned int prodCount = 1;
	for (Product prod : vecProducts)
	{
		std::string symbol = prod.getSymbol();
		int			id = prod.getID();

		std::cout << "Getting prices for " << symbol << " ";
		std::cout << prodCount << "/" << vecProducts.size();

		std::vector<Price>	vecPrices;
		bapi.getPrices(symbol, "1m", 500, vecPrices);

		unsigned int inserted = 0;
		for (Price p : vecPrices)
		{
			if (db.insertPrice(id, p))
				inserted++;
		}
		std::cout << " " << inserted << " new rows inserted" << std::endl;
		prodCount++;
	}
}

int main()
{
	// Create a database object
	DataBase db;
	if (!db.connect())
	{
		std::cout << "Failed to open the database" << std::endl;
		return 0;
	}

	WebSocketRequest wsr;
	updateProducts(wsr, db);
	getPrices(wsr, db);

	int broken;
	if (!db.verifyUploads(broken))
	{
		std::cout << "There are " << broken << " product(s) with missing data :(" << std::endl;
	}

    return 0;
}

