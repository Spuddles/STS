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

bool DataBase::getProductCount(int &value)
{
	std::string sql = "SELECT COUNT(*) from tblProducts";

	return (getScaler(sql, value));
}

bool DataBase::getActiveProductCount(int &value)
{
	std::string sql = "SELECT COUNT(*) FROM tblProducts WHERE ACTIVE = 1";

	return (getScaler(sql, value));
}

bool DataBase::getAllProducts(std::vector<Product> &vecProducts)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, active, baseAsset, baseAssetName, baseAssetUnit, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteAsset, quoteAssetName, quoteAssetUnit, status, symbol, tickSize, withdrawFee "
		"FROM tblProducts WHERE active=1";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			Product p;
			p.setID(sqlite3_column_int(pStatement, 1));
			p.setActive(sqlite3_column_int(pStatement, 2));
			p.setBaseAsset(sqlite3_column_text(pStatement, 3));
			p.setBaseAssetName(sqlite3_column_text(pStatement, 4));
			p.setBaseAssetUnit(sqlite3_column_text(pStatement, 5));
			p.setDecimalPlaces(sqlite3_column_int(pStatement, 6));
			p.setMatchingUnitType(sqlite3_column_text(pStatement, 7));
			p.setMinQty(sqlite3_column_int(pStatement, 8));
			p.setMinTrade(sqlite3_column_int(pStatement, 9));
			p.setQuoteAsset(sqlite3_column_text(pStatement, 10));
			p.setQuoteAssetName(sqlite3_column_text(pStatement, 11));
			p.setQuoteAssetUnit(sqlite3_column_text(pStatement, 12));
			p.setStatus(sqlite3_column_text(pStatement, 13));
			p.setSymbol(sqlite3_column_text(pStatement, 14));
			p.setTickSize(sqlite3_column_int(pStatement, 15));
			p.setWithdrawFee(sqlite3_column_double(pStatement, 16));

			vecProducts.push_back(p);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
}

bool DataBase::insertProduct(Product p)
{
	bool success = false;

	std::string sql = "INSERT INTO tblProducts (active, baseAsset, baseAssetName, baseAssetUnit, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteAsset, quoteAssetName, quoteAssetUnit, status, symbol, tickSize, withdrawFee) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		retCode = sqlite3_bind_int(pStatement, 1, p.isActive());
		retCode = sqlite3_bind_text(pStatement, 2, p.getBaseAsset().c_str(), -1, SQLITE_TRANSIENT);
		retCode = sqlite3_bind_text(pStatement, 3, p.getBaseAssetName().c_str(), -1, SQLITE_TRANSIENT);
		retCode = sqlite3_bind_text(pStatement, 4, p.getBaseAssetUnit().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(pStatement, 5, p.getDecimalPlaces());
		sqlite3_bind_text(pStatement, 6, p.getMatchingUnitType().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(pStatement, 7, p.getMinQty());
		sqlite3_bind_int(pStatement, 8, p.getMinTrade());
		sqlite3_bind_text(pStatement, 9, p.getQuoteAsset().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 10, p.getQuoteAssetName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 11, p.getQuoteAssetUnit().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 12, p.getStatus().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 13, p.getSymbol().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int(pStatement, 14, p.getTickSize());
		sqlite3_bind_double(pStatement, 15, p.getWithdrawFee());

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}

	return success;
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

bool DataBase::executeNonQuery(std::string sql)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}