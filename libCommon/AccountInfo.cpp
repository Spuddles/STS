#include "AccountInfo.h"

using namespace STS;

AccountInfo::AccountInfo():
	m_bCanTrade(false),
	m_bCanWithdraw(false),
	m_bCanDeposit(false)
{
}

AccountInfo::~AccountInfo()
{
}

void AccountInfo::insertHolding(const std::string &coin, double &available, double &locked)
{
	Holding h(coin, available, locked);
	m_mapHoldings.insert(std::pair<std::string, Holding>(coin, h));
	m_vecHoldings.push_back(h);
}

void AccountInfo::getAllHoldings(std::vector<Holding> &vecHoldings) const
{
	vecHoldings = m_vecHoldings;
}

void AccountInfo::getActiveHoldings(std::vector<Holding> &vecHoldings) const
{
	for (Holding h : m_vecHoldings)
	{
		if ((h.m_Available + h.m_Locked) > 0.0)
			vecHoldings.push_back(h);
	}
}

bool AccountInfo::getHolding(const std::string &coin, Holding &holding) const
{
	auto it = m_mapHoldings.find(coin);
	if (it != m_mapHoldings.end())
	{
		holding = it->second;
		return true;
	}
	return false;
}

void AccountInfo::setCanTrade(bool bCanTrade)
{
	m_bCanTrade = bCanTrade;
}

bool AccountInfo::canTrade() const
{
	return m_bCanTrade;
}

void AccountInfo::setCanWithdraw(bool bCanWithdraw)
{
	m_bCanWithdraw = bCanWithdraw;
}

bool AccountInfo::canWithdraw() const
{
	return m_bCanWithdraw;
}

void AccountInfo::setCanDeposit(bool bCanDeposit)
{
	m_bCanDeposit = bCanDeposit;
}

bool AccountInfo::canDeposit() const
{
	return m_bCanDeposit;
}
