#include <fstream>
#include <streambuf>
#include <iostream>

#include "BinanceAPI.h"
#include "Product.h"
#include "DataBase.h"
#include "CURLRequest.h"
#include "Helpers.h"

using namespace STS;

void updateCoins(DataBase &db, std::vector<Product> &vecProds)
{
	bool	bExists;
	for (Product p : vecProds)
	{
		std::string name = p.getBaseAsset();
		std::string desc = p.getBaseAssetName();
		std::string token = p.getBaseAssetUnit();

		db.doesCoinExist(name, bExists);
		if (!bExists)
		{
			db.insertCoin(name, desc, token);
		}

		name = p.getQuoteAsset();
		desc = p.getQuoteAssetName();
		token = p.getQuoteAssetUnit();

		db.doesCoinExist(name, bExists);
		if (!bExists)
		{
			db.insertCoin(name, desc, token);
		}
	}
}

void updateProducts(CURLRequest &cr, DataBase &db)
{
	BinanceAPI bapi(&cr);

	// Set all the products as inactive
	if (!db.clearActiveFlag())
	{
		std::cout << "Failed to clear active flags" << std::endl;
	}

	// Get all the products from binance
	std::vector<Product> vecProds;
	bapi.getProducts(vecProds);
	std::cout << "Received " << vecProds.size() << " products from binance" << std::endl;

	// Get all the coins we have defined
	updateCoins(db, vecProds);
	std::map<std::string, Coin>	mapCoins;
	db.getAllCoinsMap(mapCoins);
	std::cout << "We have " << mapCoins.size() << " unique cryptocoins" << std::endl;

	// Insert or update them in the DB
	unsigned int updated=0, inserted=0;
	for (Product p : vecProds)
	{
		Coin bc = mapCoins.find(p.getBaseAsset())->second;
		Coin qc = mapCoins.find(p.getQuoteAsset())->second;
		p.setBaseCoin(bc.getID());
		p.setQuoteCoin(qc.getID());

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

void getPrices(CURLRequest &cr, DataBase &db)
{
	BinanceAPI bapi(&cr);

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
		bapi.getHistoricPrices(symbol, "1m", 500, vecPrices);

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
	if (!db.connect("Trading.sqlite"))
	{
		std::cout << "Failed to open the database" << std::endl;
		return 0;
	}

	CURLRequest cr;
	updateProducts(cr, db);
	getPrices(cr, db);

	int broken;
	if (!db.verifyUploads(broken))
	{
		std::cout << "There are " << broken << " product(s) with missing data :(" << std::endl;
	}
    return 0;
}

