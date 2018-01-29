#pragma once
#include <string>

class Order
{
public:
	Order(unsigned int id, double amount, double price, bool bLimit = true) :
		m_ID(id), m_Amount(amount), m_Price(price), m_bLimit(bLimit) {}
	virtual ~Order() {};

	unsigned int	getID() const { return m_ID; }
	double			getAmount() const { return m_Amount; }
	double			getPrice() const { return m_Price; }
	bool			isBuy() const { return m_Amount >= 0.0; }
	bool			isSell() const { return m_Amount <= 0.0; };
	bool			isLimitOrder() const { return m_bLimit; }

private:
	unsigned int	m_ID;
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

