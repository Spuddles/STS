#pragma once
#include <map>
#include <vector>
#include <string>
#include "Orders.h"

namespace STS
{
	class Price;
	class Product;

	class Portfolio
	{
	public:
				Portfolio(double transactionFee);
		virtual ~Portfolio();

		void	initialiseFunds(std::vector<Product> &vecProducts);
		void	initialiseFunds(const Product &product);
		void	updatePrice(const Price &price);

		void	addFunds(unsigned int id, double amount);
		void	addFilledOrder(const Order &order);

		bool	canAfford(const Product &prod, double amount, double price);

		double	getAmount(unsigned int coinID);
		double	getBTCValue();

		void	displayPosition();
		void	displayHistory();

	private:
		double							m_price;
		double							m_transactionFee;
		std::map<unsigned int, double>	m_mapCurrentPosition;
		std::vector<Order>				m_vecFilledOrders;
	};
} // namespace STS