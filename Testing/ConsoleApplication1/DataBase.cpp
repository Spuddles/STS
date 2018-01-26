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

bool DataBase::clearActiveFlag()
{
	std::string sql = "UPDATE tblProducts SET active=0";

	return (executeNonQuery(sql));
}

bool DataBase::getAllProducts(std::vector<Product> &vecProducts)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, active, baseAsset, baseAssetName, baseAssetUnit, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteAsset, quoteAssetName, quoteAssetUnit, status, symbol, tickSize, withdrawFee "
		"FROM tblProducts";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			Product p;
			p.setID(sqlite3_column_int(pStatement, 0));
			p.setActive( (sqlite3_column_int(pStatement, 1)==1) );
			p.setBaseAsset(sqlite3_column_text(pStatement, 2));
			p.setBaseAssetName(sqlite3_column_text(pStatement, 3));
			p.setBaseAssetUnit(sqlite3_column_text(pStatement, 4));
			p.setDecimalPlaces(sqlite3_column_int(pStatement, 5));
			p.setMatchingUnitType(sqlite3_column_text(pStatement, 6));
			p.setMinQty(sqlite3_column_int(pStatement, 7));
			p.setMinTrade(sqlite3_column_int(pStatement, 8));
			p.setQuoteAsset(sqlite3_column_text(pStatement, 9));
			p.setQuoteAssetName(sqlite3_column_text(pStatement, 10));
			p.setQuoteAssetUnit(sqlite3_column_text(pStatement, 11));
			p.setStatus(sqlite3_column_text(pStatement, 12));
			p.setSymbol(sqlite3_column_text(pStatement, 13));
			p.setTickSize(sqlite3_column_int(pStatement, 14));
			p.setWithdrawFee(sqlite3_column_double(pStatement, 15));

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
		sqlite3_bind_double(pStatement, 7, p.getMinQty());
		sqlite3_bind_double(pStatement, 8, p.getMinTrade());
		sqlite3_bind_text(pStatement, 9, p.getQuoteAsset().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 10, p.getQuoteAssetName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 11, p.getQuoteAssetUnit().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 12, p.getStatus().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 13, p.getSymbol().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 14, p.getTickSize());
		sqlite3_bind_double(pStatement, 15, p.getWithdrawFee());

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);

		// Get the unique row id
		int id = (int)sqlite3_last_insert_rowid(m_pSQLiteDB);
		p.setID(id);
	}

	return success;
}

bool DataBase::updateProduct(const Product &p)
{
	bool success = false;

	std::string sql = "UPDATE tblProducts SET active=?, baseAsset=?, baseAssetName=?, baseAssetUnit=?, decimalPlaces=?,"
		"matchingUnitType=?,minQty=?, minTrade=?, quoteAsset=?, quoteAssetName=?, quoteAssetUnit=?, status=?, symbol=?,"
		"tickSize=?, withdrawFee=?) WHERE id=?";

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
		sqlite3_bind_double(pStatement, 7, p.getMinQty());
		sqlite3_bind_double(pStatement, 8, p.getMinTrade());
		sqlite3_bind_text(pStatement, 9, p.getQuoteAsset().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 10, p.getQuoteAssetName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 11, p.getQuoteAssetUnit().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 12, p.getStatus().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 13, p.getSymbol().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 14, p.getTickSize());
		sqlite3_bind_double(pStatement, 15, p.getWithdrawFee());
		sqlite3_bind_int(pStatement, 16, p.getID());

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}

bool DataBase::getNextUpdateTime(int64_t &value)
{
	std::string sql = "select MAX(openTime) AS mot from tblOneMinutePrices2 GROUP BY ID ORDER BY mot";

	return (getScaler64(sql, value));
}

bool DataBase::insertPrice(int id, Price &price)
{
	bool success = false;

	std::string sql = "INSERT INTO tblOneMinutePrices2 (id, openTime, open, high, low, close,"
		"volume, closeTime, trades) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		sqlite3_bind_int(	pStatement, 1, id);
		sqlite3_bind_int64(	pStatement, 2, price.getOpenTime());
		sqlite3_bind_double(pStatement, 3, price.getOpen());
		sqlite3_bind_double(pStatement, 4, price.getHigh());
		sqlite3_bind_double(pStatement, 5, price.getLow());
		sqlite3_bind_double(pStatement, 6, price.getClose());
		sqlite3_bind_double(pStatement, 7, price.getVolume());
		sqlite3_bind_int64(	pStatement, 8, price.getCloseTime());
		sqlite3_bind_int64(	pStatement, 9, price.getTrades());

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}

	return success;
}


bool DataBase::getScaler(const std::string &sql, int &retValue)
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

bool DataBase::getScaler64(const std::string &sql, int64_t &retValue)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_ROW)
		{
			retValue = sqlite3_column_int64(pStatement, 1);
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}

bool DataBase::executeNonQuery(const std::string &sql)
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