#include "Portfolio.h"
#include <iostream>
#include "Price.h"
#include "Product.h"

Portfolio::Portfolio()
{
	m_mapPrices[2] = 11.3088;   // BNB USD
	m_mapPrices[4] = 10354.97;	// BTC USD
}

Portfolio::~Portfolio()
{
}

void Portfolio::addFunds(unsigned int id, double amount)
{
	auto it = m_mapCurrentPosition.find(id);
	if (it != m_mapCurrentPosition.end())
	{
		m_mapCurrentPosition[id] = amount;
	}
	else
	{
		m_mapCurrentPosition[id] += amount;
	}
}

void Portfolio::addFilledOrder(const Order &order)
{
	// Make sure we have postions for each coin
	Product prod = order.getProduct();

	auto it = m_mapCurrentPosition.find(prod.getBaseCoin());
	if (it == m_mapCurrentPosition.end())
	{
		m_mapCurrentPosition[prod.getBaseCoin()] = 0;
	}
	it = m_mapCurrentPosition.find(prod.getQuoteCoin());
	if (it == m_mapCurrentPosition.end())
	{
		m_mapCurrentPosition[prod.getQuoteCoin()] = 0;
	}

	if (order.isBuy())
	{
		// Increasing base, decreasing quote
		m_mapCurrentPosition[prod.getBaseCoin()] += order.getAmount();
		m_mapCurrentPosition[prod.getQuoteCoin()] -= order.getAmount()*order.getPrice();
	}
	else
	{
		// Decreasing the base, increasing the quote
		m_mapCurrentPosition[prod.getBaseCoin()] += order.getAmount(); // This will be a negative number hance the add (+)
		m_mapCurrentPosition[prod.getQuoteCoin()] -= order.getAmount()*order.getPrice();
	}
	m_vecFilledOrders.push_back(order);
}

bool Portfolio::canAfford(const Product &prod, double amount, double price)
{
	if (amount > 0.0)
	{
		// Buying
		int coinID = prod.getQuoteCoin();
		double value = amount * price;
		return (m_mapCurrentPosition[coinID] > value);
	}
	else
	{
		// Selling
		int coinID = prod.getBaseCoin();
		return (m_mapCurrentPosition[coinID] > amount);
	}
}

double Portfolio::getAmount(unsigned int coinID)
{
	return m_mapCurrentPosition[coinID];
}

double Portfolio::getUSDValue()
{
	double	totalValue = 0.0;
	for (auto it : m_mapCurrentPosition)
	{
		unsigned int id = it.first;
		double		amount = it.second;
		double value = m_mapPrices[id] * amount;
		totalValue += value;
	}
	return totalValue;
}

void Portfolio::displayPosition()
{
	double	totalValue = 0.0;
	for (auto it : m_mapCurrentPosition)
	{
		unsigned int id = it.first;
		double		amount = it.second;
		double value = m_mapPrices[id] * amount;
		std::cout << id << " = " << amount << " (" << value << ")" << std::endl;
		totalValue += value;
	}
	std::cout << "Total Value = " << totalValue << " USD" << std::endl;
}

void Portfolio::displayHistory()
{
	for (Order o : m_vecFilledOrders)
	{
		std::cout << o.getProduct().getID() << "," << o.getAmount() << "," << o.getPrice() << std::endl;
	}
}