#include "stdafx.h"
#include "ProductCollection.h"

ProductCollection::ProductCollection(DataBase *pDatabase):
	m_pDatabase(pDatabase)
{
}

ProductCollection::~ProductCollection()
{
}

bool ProductCollection::getProductID(std::string symbol, uint32_t &productID)
{
	auto it = m_mapSymbolToID.find(symbol);
	if (it != m_mapSymbolToID.end())
	{
		productID = 1;
		return true;
	}
	return false;
}

bool ProductCollection::getProduct(uint32_t	productID, Product &product)
{
	return false;
}

bool ProductCollection::getProduct(std::string symbol, Product &product)
{
	return false;
}

bool ProductCollection::loadFromDatabase()
{
	std::string sql = "SELECT STUFF FROM TBLPRODUCTS";
	return false;
}

bool ProductCollection::addToDatabase(Product &product)
{
	// Write the product to the database
	if (!m_pDatabase->insertProduct(product))
	{
		return false;
	}

	// Load the product so we can get the new id
	return true;
}

bool ProductCollection::doesSymbolExist(std::string symbol)
{
	auto it = m_mapSymbolToID.find(symbol);

	return (it != m_mapSymbolToID.end());
}
