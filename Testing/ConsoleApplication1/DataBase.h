#pragma once
#include "sqlite3.h"
#include <vector>
#include <string>
#include "Product.h"
#include "Price.h"

class DataBase
{
public:
			DataBase();
	virtual ~DataBase();

	bool	connect();

	// PRODUCTS
	bool	getProductCount(int &value);
	bool	getActiveProductCount(int &value);
	bool	clearActiveFlag();
	bool	getAllProducts(std::vector<Product> &vecProducts);
	bool	insertProduct(Product p);
	bool	updateProduct(const Product &p);

	// PRICES
	bool	insertPrice(int id, Price &price);
	bool	getNextUpdateTime(int64_t &value);

private:
	bool	getScaler(const std::string &sql, int &retValue);
	bool	getScaler64(const std::string &sql, int64_t &retValue);
	bool	executeNonQuery(const std::string &sql);

	sqlite3	*m_pSQLiteDB;
};

