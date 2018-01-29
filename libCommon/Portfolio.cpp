#include "Portfolio.h"
#include <iostream>
#include "Price.h"

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
	auto it = m_mapCurrentPosition.find(order.getID());
	if (it != m_mapCurrentPosition.end())
	{
		m_mapCurrentPosition[order.getID()] += order.getAmount();
	}
	else
	{
		m_mapCurrentPosition[order.getID()] = order.getAmount();
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
		std::cout << o.getID() << "," << o.getAmount() << "," << o.getPrice() << std::endl;
	}
}
