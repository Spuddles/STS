#include "CrumbleStrategy.h"
#include "Price.h"
#include <iostream>
#include <sstream>
#include "Logger.h"

using namespace STS;

CrumbleStrategy::CrumbleStrategy(int intervalsBetweenOrders) :
	m_BB(21, 2.0), m_Gradient(5), m_BuySignalCount(0), m_SellSignalCount(0),
	m_intervalsBetweenOrders(intervalsBetweenOrders), m_countDown(0)
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

	if (--m_countDown > 0)
	{
		//m_bBBBuyFlag = false;
		//m_bBBSellFlag = false;
	}
	else
	{
		// See if the price is lower than bound to signal a cheap price to buy
		if (price.getClose() < m_BB.getLowerValue())
		{
			m_bBBBuyFlag = true;
			m_bBBSellFlag = false;
			m_countDown = 5;
		}

		// See if the price is higher than the upper bound to signal overpriced and good to sell
		if (price.getClose() > m_BB.getUpperValue())
		{
			m_bBBSellFlag = true;
			m_bBBBuyFlag = false;
			m_countDown = 5;
		}
	}

	std::stringstream ss;
	ss << priceCount++ << ", " << m_BB.getLowerValue() << ", " << price.getClose();
	ss << ", " << m_BB.getMidValue() << ", " << m_BB.getUpperValue() << ", ";
	if (isSellSignal())
		ss << price.getClose();
	ss << ", ";
	if (isBuySignal())
		ss << price.getClose();
	ss << ", " << m_Gradient.getValue();
	Log(ALGO, ss.str());
}

bool CrumbleStrategy::isBuySignal()
{
	bool bRet = (m_bBBBuyFlag && m_Gradient.isGoingUp());
	return bRet;
}

bool CrumbleStrategy::isSellSignal()
{
	bool bRet = (m_bBBSellFlag && m_Gradient.isGoingDown());
	return bRet;
}
