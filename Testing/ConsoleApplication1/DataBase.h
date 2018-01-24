#pragma once
#include "sqlite3.h"
#include <vector>
#include <string>
#include "Product.h"

class DataBase
{
public:
			DataBase();
	virtual ~DataBase();

	bool	connect();
	bool	getProductCount(int &value);
	bool	getActiveProductCount(int &value);

	bool	getAllProducts(std::vector<Product> &vecProducts);

	bool	insertProduct(Product p);

private:
	bool	getScaler(std::string sql, int &retValue);
	bool	executeNonQuery(std::string sql);

	sqlite3	*m_pSQLiteDB;
};

