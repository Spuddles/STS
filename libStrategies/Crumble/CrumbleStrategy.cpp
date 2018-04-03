#include "CrumbleStrategy.h"
#include "Price.h"
#include "Params.h"
#include <iostream>
#include <sstream>
#include "Logger.h"
#include <iomanip>
#include "Helpers.h"

using namespace STS;

CrumbleStrategy::CrumbleStrategy():
	m_BB(21, 2.0), m_tradeTrigger(0.0),
	m_minOrderGap(0), m_eNextTrade(BUY), m_lastOrderTime(0L)
{
}

CrumbleStrategy::~CrumbleStrategy()
{
}

double CrumbleStrategy::getBBValue(const Price &price)
{
	m_BB.updatePrice(price.getClose());
	if (price.getClose() < m_BB.getLowerValue())
	{
		// Signal to buy
		return 1.0;
	}
	else if (price.getClose() > m_BB.getUpperValue())
	{
		// Signal to sell
		return 1.0;
	}
	return 0.0;
}

double CrumbleStrategy::getPriceMovementValue(const Price &price)
{
	static int count = 0;

	if (m_tradePrice < 0.0)
		m_tradePrice = price.getClose();


	// If we haven't traded for 30 minutes, reset the last trade price
	if ((price.getCloseTime() - m_lastOrderTime) / (60000 * 360))
	{
		m_tradePrice = price.getClose();
		m_lastOrderTime = price.getCloseTime() - (60000 * 300);
	}

	double diff = (m_tradePrice - price.getClose()) / m_tradePrice;
	return (diff / 0.05);
}

double CrumbleStrategy::getMinGapValue(const Price &price)
{
	uint64_t diff = price.getCloseTime() - m_lastOrderTime;

	int minutes = (int)(diff / (1000 * 60));

	if (minutes > m_minOrderGap)
	{
		return 1.0;
	}
	return 0.0;
}

void CrumbleStrategy::initialise(const Params &p)
{
	if (!p.getDouble("tradeTrigger", m_tradeTrigger))
	{
		Log(CRITICAL, "Failed to get tradeTrigger value");
		return;
	}

	if (!p.getInt("minOrderGap", m_minOrderGap))
	{
		Log(CRITICAL, "Failed to get minOrderGap value");
		return;
	}
}

void CrumbleStrategy::updatePrice(const Price &price)
{
	static unsigned int priceCount = 0;

	double bbSignal = getBBValue(price);
	double priceMoveSignal = getPriceMovementValue(price);
	double minGapSignal = getMinGapValue(price);

	double result = (bbSignal * minGapSignal);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(6) << std::setw(5);
	ss << priceCount << ", " << bbSignal << ", ";
	ss << priceMoveSignal << ", " << minGapSignal;

	Log(SIGNAL, ss.str());

	m_bBBBuyFlag = false;
	m_bBBSellFlag = false;

	// See if the price is lower than bound to signal a cheap price to buy
	if ( (result - priceMoveSignal)  > m_tradeTrigger && (m_eNextTrade == BUY || m_eNextTrade == EITHER))
	{
		m_bBBBuyFlag = true;
		m_bBBSellFlag = false;
	}

	// See if the price is higher than the upper bound to signal overpriced and good to sell
	if ( (result + priceMoveSignal) > m_tradeTrigger && (m_eNextTrade == SELL || m_eNextTrade == EITHER))
	{
		m_bBBSellFlag = true;
		m_bBBBuyFlag = false;
	}

	ss = std::stringstream("");
	ss << std::setw(5) << priceCount++ << ", ";
	ss << Helpers::convertTime(price.getCloseTime()) << ", ";
	ss << m_BB.getLowerValue() << ", ";
	ss << price.getClose() << ", ";
	ss << m_BB.getMidValue() << ", ";
	ss << m_BB.getUpperValue() << ", ";
	if (isSellSignal())
		ss << price.getClose();
	ss << ", ";
	if (isBuySignal())
		ss << price.getClose();
	Log(ALGO, ss.str());
}

bool CrumbleStrategy::isBuySignal() const
{
	bool bRet = (m_bBBBuyFlag);
	return bRet;
}

bool CrumbleStrategy::isSellSignal() const
{
	bool bRet = (m_bBBSellFlag);
	return bRet;
}

void CrumbleStrategy::resetBuySell()
{
	m_eNextTrade = EITHER;
}

void CrumbleStrategy::bought(const Price &price)
{
//	std::cout << "bought" << std::endl;
	Log(TRADE, "Buy order complete");
	m_tradePrice = price.getClose();
	m_eNextTrade = SELL;
	m_lastOrderTime = price.getCloseTime();
}

void CrumbleStrategy::sold(const Price &price)
{
//	std::cout << "sold" << std::endl;
	Log(TRADE, "Sell order complete");
	m_tradePrice = price.getClose();
	m_eNextTrade = BUY;
	m_lastOrderTime = price.getCloseTime();
}
