#pragma once
#include "IStrategy.h"
#include "BollingerBand.h"
#include "Gradient.h"

namespace STS
{
	class CrumbleStrategy : public IStrategy
	{
	public:
		CrumbleStrategy(int intervalsBetweenOrders);
		virtual ~CrumbleStrategy();

		void	updatePrice(const Price &price);
		bool	isBuySignal();
		bool	isSellSignal();

	private:
		BollingerBand	m_BB;
		Gradient		m_Gradient;
		int				m_BuySignalCount;
		int				m_SellSignalCount;

		bool			m_bBBBuyFlag;
		bool			m_bBBSellFlag;

		int				m_intervalsBetweenOrders;
		int				m_countDown;
	};
} // namespace STS
