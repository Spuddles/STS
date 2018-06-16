#pragma once
#include "Orders.h"
#include <string>
#include <vector>
#include <map>

namespace STS
{
	class AccountInfo;
	class Price;
	class Product;

	class Exchange
	{
	public:
		Exchange(AccountInfo &ai);
		virtual ~Exchange();

		void	updatePrice(unsigned int id, const Price &p);
		bool	placeLimitOrder(Product &product, double amount, const Price &price);
		bool	placeMarketOrder(Product &product, double amount);

		int		getLiveOrderCount(unsigned int id);
		int		minutesSinceLastTrade(uint64_t now);

	private:
		AccountInfo						&m_AccountInfo;
		std::map<unsigned int, Price>	m_mapPrices;
		std::vector<Order>				m_vecOrders;
		uint64_t						m_lastTradeTime;
	};
} // namespace STS
