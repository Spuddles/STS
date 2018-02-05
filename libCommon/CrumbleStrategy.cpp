#include "CrumbleStrategy.h"
#include "Price.h"
#include <iostream>
#include <sstream>
#include "Logger.h"

CrumbleStrategy::CrumbleStrategy():
	m_BB(21, 2.0), m_Gradient(1), m_BuySignalCount(0), m_SellSignalCount(0)
{
}

CrumbleStrategy::~CrumbleStrategy()
{
}

void CrumbleStrategy::updatePrice(const Price &price)
{
	static unsigned int priceCount = 0;
	m_BB.updatePrice(price.getClose());
	m_Gradient.updatePrice(price.getClose());

	// See if the price is lower than bound to signal a cheap price to buy
	if (price.getClose() < m_BB.getLowerValue() && m_Gradient.isGoingUp())
	{
		m_BuySignalCount++;
	}
	else
	{
		m_BuySignalCount = 0;
	}

	// See if the price is higher than the upper bound to signal overpriced and good to sell
	if (price.getClose() > m_BB.getUpperValue() && m_Gradient.isGoingDown())
	{
		m_SellSignalCount++;
	}
	else
	{
		m_SellSignalCount = 0;
	}

	std::stringstream ss;
	ss << priceCount++ << ", " << m_BB.getLowerValue() << ", " << price.getClose();
	ss << ", " << m_BB.getMidValue() << ", " << m_BB.getUpperValue() << ", ";
	if (m_SellSignalCount > 0)
		ss << price.getClose();
	ss << ", ";
	if (m_BuySignalCount > 0)
		ss << price.getClose();
	Log(ALGO, ss.str());
}

bool CrumbleStrategy::isBuySignal()
{
	return (m_BuySignalCount > 0);
}

bool CrumbleStrategy::isSellSignal()
{
	return (m_SellSignalCount > 0);
}
