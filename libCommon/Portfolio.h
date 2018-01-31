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

	void	addFunds(unsigned int id, double amount);
	void	addFilledOrder(const Order &order);

	bool	canAfford(const Product &prod, double amount, double price);

	double	getAmount(unsigned int coinID);
	double	getUSDValue();

	void	displayPosition();
	void	displayHistory();

private:
	std::map<unsigned int, double>	m_mapPrices;
	std::map<unsigned int, double>	m_mapCurrentPosition;
	std::vector<Order>				m_vecFilledOrders;
};

