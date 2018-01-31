#pragma once
#include "sqlite3.h"
#include <vector>
#include <map>
#include <string>
#include "Product.h"
#include "Price.h"
#include "Coin.h"

class DataBase
{
public:
			DataBase();
	virtual ~DataBase();

	bool	connect();

	// COINS
	bool	insertCoin(const std::string &name, const std::string &description, const std::string &token);
	bool	updateCoin(const Coin &c);
	bool	doesCoinExist(const std::string &name, bool &bExists);
	bool	getAllCoins(std::vector<Coin> &vecCoins);
	bool	getAllCoinsMap(std::map<std::string, Coin> &mapCoins);

	// PRODUCTS
	bool	getProductCount(int &value);
	bool	getActiveProductCount(int &value);
	bool	deleteProducts();
	bool	clearActiveFlag();
	bool	getAllProducts(std::vector<Product> &vecProducts);
	bool	getProduct(unsigned int id, Product &product);
	bool	insertProduct(Product p);
	bool	updateProduct(const Product &p);
	bool	doesProductExist(const std::string &symbol, bool &exists);
	bool	getProductID(const std::string &symbol, int &id);

	// PRICES
	bool	insertPrice(int id, Price &price);
	bool	getNextUpdateTime(int64_t &value);
	bool	verifyUploads(int &broken);
	bool	getPriceData(int id, std::vector<Price> &vecPrices);

private:
	bool	getScaler(const std::string &sql, int &retValue);
	bool	getScaler64(const std::string &sql, int64_t &retValue);
	bool	executeNonQuery(const std::string &sql);
	bool	setPragma(const std::string &pragma, const std::string &value);

	sqlite3	*m_pSQLiteDB;
};

