#pragma once
#include "Product.h"
#include "DataBase.h"
#include <map>

class ProductCollection
{
public:
				ProductCollection(DataBase *pDatabase);
	virtual		~ProductCollection();

	bool		getProductID(std::string symbol, uint32_t &productID);
	bool		getProduct(uint32_t	productID, Product &product);
	bool		getProduct(std::string symbol, Product &product);

	bool		loadFromDatabase();
	bool		addToDatabase(Product &product);
	bool		doesSymbolExist(std::string symbol);

private:
	// Map for quick lookup of ID from symbol name
	std::map<std::string, uint32_t>	m_mapSymbolToID;

	// Map containing all the products
	std::map<uint32_t, Product>		m_mapProducts;

	DataBase	*m_pDatabase;
};

