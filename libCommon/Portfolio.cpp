#include "Portfolio.h"
#include <iostream>
#include "Price.h"
#include "Product.h"

Portfolio::Portfolio()
{
}

Portfolio::~Portfolio()
{
}

void Portfolio::updatePrice(unsigned int id, const Price &p)
{
	m_mapPrices[id] = p.getClose();
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

void Portfolio::displayPosition()
{
	for (auto it : m_mapCurrentPosition)
	{
		unsigned int id = it.first;
		double		amount = it.second;
		double value = m_mapPrices[id] * amount;
		std::cout << id << " = " << amount << " (" << value << ")" << std::endl;
	}
}

void Portfolio::displayHistory()
{
	for (Order o : m_vecFilledOrders)
	{
		std::cout << o.getProduct().getID() << "," << o.getAmount() << "," << o.getPrice() << std::endl;
	}
}
