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
	bool	getProductCount();
	bool	getAllProducts(std::vector<Product> &vecProducts);

private:
	bool	getScaler(std::string sql, int &retValue);

	sqlite3	*m_pSQLiteDB;
};

