#pragma once
#include "IStrategy.h"
#include "BollingerBand.h"
#include "Gradient.h"

namespace STS
{
	class CrumbleStrategy : public IStrategy
	{
		enum eNEXTTRADE
		{
			BUY,
			SELL,
			EITHER
		};

	public:
		CrumbleStrategy(double tradeTrigger, int minOrderGap);
		virtual ~CrumbleStrategy();

		void	updatePrice(const Price &price);
		bool	isBuySignal() const;
		bool	isSellSignal() const;
		void	resetBuySell();
		void	bought(const Price &price);
		void	sold(const Price &price);

	private:
		double getBBValue(const Price &price);
		double getPriceMovementValue(const Price &price);
		double getMinGapValue(const Price &price);

		BollingerBand	m_BB;

		double			m_tradeTrigger;

		int				m_minOrderGap;

		bool			m_bBBBuyFlag;
		bool			m_bBBSellFlag;

		eNEXTTRADE		m_eNextTrade;

		double			m_tradePrice=-1.0;

		uint64_t		m_lastOrderTime;
	};
} // namespace STS
