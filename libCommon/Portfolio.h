#pragma once
#include <map>
#include <vector>
#include <string>
#include "Orders.h"

class Price;

class Portfolio
{
public:
			Portfolio();
	virtual ~Portfolio();

	void	updatePrice(unsigned int id, const Price &p);

	void	addFunds(unsigned int id, double amount);
	void	addFilledOrder(const Order &order);

	void	displayPosition();
	void	displayHistory();

private:
	std::map<unsigned int, double>	m_mapPrices;
	std::map<unsigned int, double>	m_mapCurrentPosition;
	std::vector<Order>				m_vecFilledOrders;
};

