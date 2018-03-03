#include "Exchange.h"
#include "Portfolio.h"
#include "Price.h"
#include "Product.h"
#include <iostream>
#include "Logger.h"

using namespace STS;

Exchange::Exchange(Portfolio &p) :
	m_Portfolio(p),
	m_lastTradeTime(0LL)
{
}

Exchange::~Exchange()
{
}

void Exchange::updatePrice(unsigned int id, const Price &p)
{
	// Set the new price
	m_mapPrices[id] = p;

	// Look for any orders we can fill
	for (size_t i=0;i<m_vecOrders.size();i++)
	{
		Order o = m_vecOrders[i];
		if (o.getProduct().getID() == id)
		{
			if (o.isBuy() && o.getPrice() > p.getLow())
			{
				if (o.getAmount() < p.getVolume())
				{
					m_Portfolio.addFilledOrder(o);
					//m_Portfolio.displayPosition();
					m_vecOrders.erase(m_vecOrders.begin() + i);
					--i;
				}
				else
				{
					Log(EXCHANGE, "Not enough liquidity to fill order");
				}
			}
			else if (o.isSell() && o.getPrice() < p.getHigh())
			{
				if (o.getAmount() < p.getVolume())
				{
					// Move this order to the filled collection
					m_Portfolio.addFilledOrder(o);
					//m_Portfolio.displayPosition();
					m_vecOrders.erase(m_vecOrders.begin() + i);
					--i;
				}
				else
				{
					Log(EXCHANGE, "Not enough liquidity to fill order");
				}
			}
		}
		if (!m_vecOrders.empty())
		{
			Log(EXCHANGE, "Orders left in exchange unfilled");
		}
	}
}

bool Exchange::placeLimitOrder(Product &product, double amount, const Price &price)
{
//	std::cout << "Placing order on " << product.getID() << " for " << amount << " at price " << price << std::endl;
	m_vecOrders.push_back(Order(product, amount, price.getClose(), true));

	m_lastTradeTime = price.getCloseTime();

	return true;
}

bool Exchange::placeMarketOrder(Product &product, double amount)
{
	m_vecOrders.push_back(Order(product, amount, 0.0, false));
	return true;
}

int Exchange::getLiveOrderCount(unsigned int id)
{
	int orderCount = 0;
	for (Order &o : m_vecOrders)
	{
		if (o.getProduct().getID() == id)
		{
			orderCount++;
		}
	}
	return orderCount;
}

int Exchange::minutesSinceLastTrade(uint64_t now)
{
	int minutes = (int)((now - m_lastTradeTime) / 60000);
	return minutes;
}

