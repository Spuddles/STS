#pragma once
#include "Orders.h"
#include <string>
#include <vector>
#include <map>

class Portfolio;
class Price;

class Exchange
{
public:
			Exchange(Portfolio &p);
	virtual ~Exchange();

	void	updatePrice(unsigned int id, const Price &p);
	bool	placeLimitOrder(unsigned int id, double amount, double price);
	bool	placeMarketOrder(unsigned int id, double amount);

	int		getLiveOrderCount(unsigned int id);

private:
	Portfolio						&m_Portfolio;
	std::map<unsigned int, Price>	m_mapPrices;
	std::vector<Order>				m_vecOrders;
};

