#pragma once
#include <string>
#include "Product.h"

class Order
{
public:
	Order(Product &product, double amount, double price, bool bLimit = true) :
		m_Product(product), m_Amount(amount), m_Price(price), m_bLimit(bLimit) {}
	virtual ~Order() {};

	Product			getProduct() const { return m_Product; }
	double			getAmount() const { return m_Amount; }
	double			getPrice() const { return m_Price; }
	bool			isBuy() const { return m_Amount >= 0.0; }
	bool			isSell() const { return m_Amount <= 0.0; };
	bool			isLimitOrder() const { return m_bLimit; }

private:
	Product			m_Product;
	unsigned int	m_QuoteID;
	unsigned int	m_BaseID;
	double			m_Amount;
	double			m_Price;
	bool			m_bLimit;
};

class FilledOrder : public Order
{
public:

private:

};

