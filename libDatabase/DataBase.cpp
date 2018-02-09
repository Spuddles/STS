#include "DataBase.h"
#include <string>
#include <sstream>
#include "Logger.h"

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

bool DataBase::connect(const std::string &dbFile)
{
	int retCode = sqlite3_open(dbFile.c_str(), &m_pSQLiteDB);
	if (retCode != SQLITE_OK)
	{
		Log(CRITICAL, "Failed to open the database file " + dbFile);
		return false;
	}

	// Make sure we have all the tables
	if (!doesTableExist("tblCoins"))
	{
		if (!createCoinTable())
		{
			Log(CRITICAL, "Failed to create the coin database table");
			return false;
		}
	}
	if (!doesTableExist("tblProducts"))
	{
		if (!createProductTable())
		{
			Log(CRITICAL, "Failed to create the product database table");
			return false;
		}
	}
	if (!doesTableExist("tblOneMinutePrices"))
	{
		if (!createPriceTable())
		{
			Log(CRITICAL, "Failed to create the prices database table");
			return false;
		}
	}

	// Set some options to speed up inserts
	bool bRet = setPragma("journal_mode", "MEMORY");
	bRet &= setPragma("synchronous", "OFF");
	return bRet;
}

bool DataBase::doesTableExist(const std::string &tablename)
{
	std::stringstream ss;
	ss << "SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name='";
	ss << tablename << "';";

	int matches;
	if (!getScaler(ss.str(), matches))
		return false;

	return (matches == 1);
}

bool DataBase::createCoinTable()
{
	std::string sql = "CREATE TABLE `tblCoins` "
		"( `id` INTEGER NOT NULL UNIQUE, "
		"`name` TEXT, "
		"`description` TEXT, "
		"`token` TEXT, "
		"PRIMARY KEY(`id`) )";

	return executeNonQuery(sql);
}

bool DataBase::createProductTable()
{
	std::string sql = "CREATE TABLE 'tblProducts' ( "
		"`Id` INTEGER NOT NULL UNIQUE, "
		"`active` INTEGER, "
		"`baseCoin` INTEGER, "
		"`decimalPlaces` INTEGER, "
		"`matchingUnitType` INTEGER, "
		"`minQty` REAL, "
		"`minTrade` REAL, "
		"`quoteCoin` INTEGER, "
		"`status` INTEGER, "
		"`symbol` TEXT ( 16 ) NOT NULL UNIQUE, "
		"`tickSize` INTEGER, "
		"`withdrawFee` REAL, "
		"FOREIGN KEY(`baseCoin`) REFERENCES `tblCoins`(`id`), "
		"FOREIGN KEY(`quoteCoin`) REFERENCES `tblCoins`(`id`), "
		"PRIMARY KEY(`Id`) )";

	return executeNonQuery(sql);
}

bool DataBase::createPriceTable()
{
	std::string sql = "CREATE TABLE 'tblOneMinutePrices' ( "
		"`id` INTEGER NOT NULL, "
		"`openTime` INTEGER, "
		"`open` REAL, "
		"`high` REAL, "
		"`low` REAL, "
		"`close` REAL, "
		"`volume` REAL, "
		"`closeTime` INTEGER, "
		"`trades` INTEGER, "
		"UNIQUE(`id`,`openTime`), "
		"FOREIGN KEY(`id`) REFERENCES `tblProducts`(`Id`) )";

	return executeNonQuery(sql);
}

bool DataBase::insertCoin(const std::string &name, const std::string &description, const std::string &token)
{
	bool success = false;
	std::string sql = "INSERT INTO tblCoins (name, description, token) VALUES (?,?,?)";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		sqlite3_bind_text(pStatement, 1, name.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 2, description.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 3, token.c_str(), -1, SQLITE_TRANSIENT);

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}

bool DataBase::updateCoin(const Coin &c)
{
	bool success = false;
	std::string sql = "UPDATE tblCoins SET name=?, description=?, token=? WHERE id=?";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		sqlite3_bind_text(pStatement, 1, c.getName().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 2, c.getDescription().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 3, c.getToken().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_int( pStatement, 3, c.getID());

		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}

bool DataBase::doesCoinExist(const std::string &name, bool &bExists)
{
	std::stringstream ss;

	ss << "SELECT COUNT(*) FROM tblCoins WHERE name = '";
	ss << name << "'";

	int iVal;
	bool bRet = getScaler(ss.str(), iVal);
	bExists = (iVal > 0);
	return bRet;
}

bool DataBase::getAllCoins(std::vector<Coin> &vecCoins)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, name, description, token FROM tblCoins";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			unsigned int	id = sqlite3_column_int(pStatement, 0);
			const char*		name = (const char*)sqlite3_column_text(pStatement, 1);
			const char*		description = (const char*)sqlite3_column_text(pStatement, 2);
			const char*		token = (const char*)sqlite3_column_text(pStatement, 3);

			Coin c(id, name, description, token);
			vecCoins.push_back(c);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
}

bool DataBase::getAllCoinsMap(std::map<std::string, Coin> &mapCoins)
{
	std::vector<Coin>	vecCoins;

	if (!getAllCoins(vecCoins))
		return false;

	for (Coin c : vecCoins)
	{
		mapCoins.insert(std::pair<std::string, Coin>(c.getName(), c));
	}
	return true;
}

bool DataBase::getCoinID(const std::string &coin, unsigned int &coinID)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id FROM tblCoins WHERE name=?";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (retCode == SQLITE_OK)
	{
		sqlite3_bind_text(pStatement, 1, coin.c_str(), -1, SQLITE_TRANSIENT);
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_ROW)
		{
			coinID = sqlite3_column_int(pStatement, 0);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
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

bool DataBase::doesProductExist(const std::string &symbol, bool &exists)
{
	std::stringstream ss;
	
	ss << "SELECT COUNT(*) FROM tblProducts WHERE symbol='";
	ss << symbol << "'";

	int iVal;
	bool bRet = getScaler(ss.str(), iVal);
	exists = (iVal > 0);
	return bRet;
}

bool DataBase::clearActiveFlag()
{
	std::string sql = "UPDATE tblProducts SET active=0";

	return (executeNonQuery(sql));
}

bool DataBase::deleteProducts()
{
	std::string sql = "DELETE FROM tblProducts";

	return (executeNonQuery(sql));
}

bool DataBase::getProductID(const std::string &symbol, int &id)
{
	std::stringstream ss;
	ss << "SELECT id FROM tblProducts WHERE symbol='";
	ss << symbol << "'";

	return (getScaler(ss.str(), id));
}

bool DataBase::getAllProducts(std::vector<Product> &vecProducts)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, active, baseCoin, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteCoin, status, symbol, tickSize, withdrawFee "
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
			p.setBaseCoin(sqlite3_column_int(pStatement, 2));
			p.setDecimalPlaces(sqlite3_column_int(pStatement, 3));
			p.setMatchingUnitType(sqlite3_column_text(pStatement, 4));
			p.setMinQty(sqlite3_column_int(pStatement, 5));
			p.setMinTrade(sqlite3_column_int(pStatement, 6));
			p.setQuoteCoin(sqlite3_column_int(pStatement, 7));
			p.setStatus(sqlite3_column_text(pStatement, 8));
			p.setSymbol(sqlite3_column_text(pStatement, 9));
			p.setTickSize(sqlite3_column_int(pStatement, 10));
			p.setWithdrawFee(sqlite3_column_double(pStatement, 11));

			vecProducts.push_back(p);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
}

bool DataBase::getProducts(std::vector<std::string> &vecSymbols, std::vector<Product> &vecProducts)
{
	for (std::string symbol : vecSymbols)
	{
		int id;
		if (!getProductID(symbol, id))
		{
			return false;
		}
		Product p;
		if (!getProduct(id, p))
		{
			return false;
		}
		vecProducts.push_back(p);
	}
	return true;
}

bool DataBase::getProduct(const std::string &symbol, Product &product)
{
	int productID;
	if (!getProductID(symbol, productID))
	{
		return false;
	}
	return getProduct(productID, product);
}

bool DataBase::getProduct(unsigned int id, Product &product)
{
	bool		  success = false;
	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, active, baseCoin, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteCoin, status, symbol, tickSize, withdrawFee "
		"FROM tblProducts WHERE id=?";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	sqlite3_bind_int(pStatement, 1, id);
	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_ROW)
		{
			product.setID(sqlite3_column_int(pStatement, 0));
			product.setActive((sqlite3_column_int(pStatement, 1) == 1));
			product.setBaseCoin(sqlite3_column_int(pStatement, 2));
			product.setDecimalPlaces(sqlite3_column_int(pStatement, 3));
			product.setMatchingUnitType(sqlite3_column_text(pStatement, 4));
			product.setMinQty(sqlite3_column_int(pStatement, 5));
			product.setMinTrade(sqlite3_column_int(pStatement, 6));
			product.setQuoteCoin(sqlite3_column_int(pStatement, 7));
			product.setStatus(sqlite3_column_text(pStatement, 8));
			product.setSymbol(sqlite3_column_text(pStatement, 9));
			product.setTickSize(sqlite3_column_int(pStatement, 10));
			product.setWithdrawFee(sqlite3_column_double(pStatement, 11));

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

	std::string sql = "INSERT INTO tblProducts (active, baseCoin, decimalPlaces, matchingUnitType,"
		"minQty, minTrade, quoteCoin, status, symbol, tickSize, withdrawFee) "
		"VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		int active;
		p.isActive() ? active = 1 : active = 0;
		sqlite3_bind_int(pStatement, 1, active);
		sqlite3_bind_int(pStatement, 2, p.getBaseCoin());
		sqlite3_bind_int(pStatement, 3, p.getDecimalPlaces());
		sqlite3_bind_text(pStatement, 4, p.getMatchingUnitType().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 5, p.getMinQty());
		sqlite3_bind_double(pStatement, 6, p.getMinTrade());
		sqlite3_bind_int(pStatement, 7, p.getQuoteCoin());
		sqlite3_bind_text(pStatement, 8, p.getStatus().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 9, p.getSymbol().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 10, p.getTickSize());
		sqlite3_bind_double(pStatement, 11, p.getWithdrawFee());

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

	std::string sql = "UPDATE tblProducts SET active=?, baseCoin=?, decimalPlaces=?,"
		"matchingUnitType=?,minQty=?, minTrade=?, quoteCoin=?, status=?, symbol=?,"
		"tickSize=?, withdrawFee=? WHERE id=?";

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (SQLITE_OK == retCode)
	{
		int active;
		p.isActive() ? active = 1 : active = 0;
		sqlite3_bind_int(pStatement, 1, active);
		sqlite3_bind_int(pStatement, 2, p.getBaseCoin());
		sqlite3_bind_int(pStatement, 3, p.getDecimalPlaces());
		sqlite3_bind_text(pStatement, 4, p.getMatchingUnitType().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 5, p.getMinQty());
		sqlite3_bind_double(pStatement, 6, p.getMinTrade());
		sqlite3_bind_int(pStatement, 7, p.getQuoteCoin());
		sqlite3_bind_text(pStatement, 8, p.getStatus().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(pStatement, 9, p.getSymbol().c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_double(pStatement, 10, p.getTickSize());
		sqlite3_bind_double(pStatement, 11, p.getWithdrawFee());
		sqlite3_bind_int(pStatement, 12, p.getID());

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
	std::string sql = "select MAX(openTime) AS mot from tblOneMinutePrices GROUP BY ID ORDER BY mot";

	return (getScaler64(sql, value));
}

bool DataBase::insertPrice(int id, Price &price)
{
	bool success = false;

	std::string sql = "INSERT INTO tblOneMinutePrices (id, openTime, open, high, low, close,"
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

bool DataBase::getPriceData(const std::string &symbol, std::vector<Price> &vecPrices)
{
	int productID;
	
	if (!getProductID(symbol, productID))
	{
		return false;
	}
	return getPriceData(productID, vecPrices);
}

bool DataBase::getPriceData(std::vector<std::string> &vecSymbols, std::vector<Price> &vecPrices)
{
	std::vector<unsigned int>	vecIDs;

	int id;
	for (std::string &symbol : vecSymbols)
	{
		getProductID(symbol, id);
		vecIDs.push_back(id);
	}
	return getPriceData(vecIDs, vecPrices);
}


bool DataBase::getPriceData(int id, std::vector<Price> &vecPrices)
{
	bool success = false;

	sqlite3_stmt *pStatement;
	std::string sql = "SELECT id, openTime, open, high, low, close, volume, closeTime, trades "
		"FROM tblOneMinutePrices WHERE id=? ORDER BY openTime ASC";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, sql.c_str(), (int)sql.size(), &pStatement, nullptr);
	if (retCode == SQLITE_OK)
	{
		sqlite3_bind_int(pStatement, 1, id);

		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			Price p;
			p.setProductID(sqlite3_column_int(pStatement, 0));
			p.setOpenTime(sqlite3_column_int64(pStatement, 1));
			p.setOpen(sqlite3_column_double(pStatement, 2));
			p.setHigh(sqlite3_column_double(pStatement, 3));
			p.setLow(sqlite3_column_double(pStatement, 4));
			p.setClose(sqlite3_column_double(pStatement, 5));
			p.setVolume(sqlite3_column_double(pStatement, 6));
			p.setCloseTime(sqlite3_column_int64(pStatement, 7));
			p.setTrades(sqlite3_column_int(pStatement, 8));

			vecPrices.push_back(p);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
}

bool DataBase::getPriceData(std::vector<unsigned int> &vecIDs, std::vector<Price> &vecPrices)
{
	bool success = false;

	sqlite3_stmt *pStatement;
	std::stringstream ss;
	ss << "SELECT id, openTime, open, high, low, close, volume, closeTime, trades ";
	ss << "FROM tblOneMinutePrices WHERE id IN (";
	for (size_t i=0;i<vecIDs.size();++i)
	{
		ss << vecIDs[i];
		if (i < vecIDs.size() - 1)
		{
			ss << ",";
		}
	}
	
	ss << ") ORDER BY openTime ASC";

	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, ss.str().c_str(), (int)ss.str().size(), &pStatement, nullptr);
	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		while (retCode == SQLITE_ROW)
		{
			Price p;
			p.setProductID(sqlite3_column_int(pStatement, 0));
			p.setOpenTime(sqlite3_column_int64(pStatement, 1));
			p.setOpen(sqlite3_column_double(pStatement, 2));
			p.setHigh(sqlite3_column_double(pStatement, 3));
			p.setLow(sqlite3_column_double(pStatement, 4));
			p.setClose(sqlite3_column_double(pStatement, 5));
			p.setVolume(sqlite3_column_double(pStatement, 6));
			p.setCloseTime(sqlite3_column_int64(pStatement, 7));
			p.setTrades(sqlite3_column_int(pStatement, 8));

			vecPrices.push_back(p);
			retCode = sqlite3_step(pStatement);
		}
		sqlite3_finalize(pStatement);
		success = true;
	}
	return success;
}

bool DataBase::verifyUploads(int &broken)
{
	broken = 0;
	std::string sql = "select count(*) FROM ("
						"select count(*) AS ACTUAL,"
						"max(openTime) AS LAST,"
						"min(openTime) AS FIRST,"
						"((max(OpenTime) - min(openTime)) / 60000) + 1 AS EXPECTED "
						"FROM tblOneMinutePrices "
						"GROUP BY ID) "
					  "WHERE ACTUAL != EXPECTED";

	if (!getScaler(sql, broken))
	{
		return false;
	}
	return (broken == 0);
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
			retValue = sqlite3_column_int(pStatement, 0);
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
			retValue = sqlite3_column_int64(pStatement, 0);
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

bool DataBase::setPragma(const std::string &pragma, const std::string &value)
{
	bool		  success = false;

	std::stringstream ss;

	ss << "PRAGMA " << pragma << " = " << value;

	sqlite3_stmt *pStatement;
	int retCode = sqlite3_prepare_v2(m_pSQLiteDB, ss.str().c_str(), (int)ss.str().size(), &pStatement, nullptr);

	if (retCode == SQLITE_OK)
	{
		retCode = sqlite3_step(pStatement);
		if (retCode == SQLITE_DONE || retCode == SQLITE_ROW)
		{
			success = true;
		}
		sqlite3_finalize(pStatement);
	}
	return success;
}
