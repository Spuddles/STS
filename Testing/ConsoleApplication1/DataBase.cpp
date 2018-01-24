#include "stdafx.h"
#include "DataBase.h"
#include <string>

DataBase::DataBase():
	m_pSQLiteDB(nullptr)
{
}

DataBase::~DataBase()
{
	if (m_pSQLiteDB)
	{
		sqlite3_close(m_pSQLiteDB);
	}
}

bool DataBase::connect()
{
	int retCode = sqlite3_open("C:\\Users\\Spud\\OneDrive\\Trading\\Testing.sqlite", &m_pSQLiteDB);
	return (retCode == SQLITE_OK);
}

bool DataBase::getProductCount()
{
	std::string sql = "SELECT COUNT(*) from tblProducts";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			int value = sqlite3_column_int(pStatement, 1);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
	}
	return false;
}

bool DataBase::getAllProducts(std::vector<Product> &vecProducts)
{
	return false;
}

bool DataBase::getScaler(std::string sql, int &retValue)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_ROW)
		{
			retValue = sqlite3_column_int(pStatement, 1);
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}
