#include "Exchange.h"
#include "Portfolio.h"
#include "Price.h"
#include "Product.h"

#include <iostream>

Exchange::Exchange(Portfolio &p):
	m_Portfolio(p)
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
			bool c = o.getAmount() < p.getVolume();
			if (o.isBuy() && o.getPrice() > p.getLow() && o.getAmount() < p.getVolume())
			{
				m_Portfolio.addFilledOrder(o);
				//m_Portfolio.displayPosition();
				m_vecOrders.erase(m_vecOrders.begin() + i);
				--i;
			}
			else if (o.isSell() && o.getPrice() < p.getHigh() && o.getAmount() < p.getVolume())
			{
				// Move this order to the filled collection
				m_Portfolio.addFilledOrder(o);
				//m_Portfolio.displayPosition();
				m_vecOrders.erase(m_vecOrders.begin() + i);
				-- i;
			}
		}
	}
}

bool Exchange::placeLimitOrder(Product &product, double amount, double price)
{
//	std::cout << "Placing order on " << product.getID() << " for " << amount << " at price " << price << std::endl;
	m_vecOrders.push_back(Order(product, amount, price, true));
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
