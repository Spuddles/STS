#include "CrumbleStrategy.h"
#include "Price.h"

CrumbleStrategy::CrumbleStrategy():
	m_BB(21, 2.0), m_BuySignalCount(0), m_SellSignalCount(0)
{
}

CrumbleStrategy::~CrumbleStrategy()
{
}

void CrumbleStrategy::updatePrice(const Price &price)
{
	m_BB.addPrice(price.getClose());

	// See if the price is lower than bound to signal a cheap price to buy
	if (price.getClose() < m_BB.getLowerValue())
	{
		m_BuySignalCount++;
	}
	else
	{
		m_BuySignalCount = 0;
	}

	// See if the price is higher than the upper bound to signal overpriced and good to sell
	if (price.getClose() > m_BB.getUpperValue())
	{
		m_SellSignalCount++;
	}
	else
	{
		m_SellSignalCount = 0;
	}
}

bool CrumbleStrategy::isBuySignal()
{
	return (m_BuySignalCount > 0);
}

bool CrumbleStrategy::isSellSignal()
{
	return (m_SellSignalCount > 0);
}
