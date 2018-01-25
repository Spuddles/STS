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
	bool	getAllProducts(std::vector<Product> &vecProducts);
	bool	insertProduct(Product p);

	// PRICES
	bool	insertPrice(int id, Price &price);

private:
	bool	getScaler(std::string sql, int &retValue);
	bool	executeNonQuery(std::string sql);

	sqlite3	*m_pSQLiteDB;
};

