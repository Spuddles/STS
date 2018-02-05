#include "BollingerBand.h"

BollingerBand::BollingerBand(int timePeriods, double scale):
	m_MA(timePeriods),
	m_Variance(timePeriods),
	m_Scale(scale)
{
}

BollingerBand::~BollingerBand()
{
}

double BollingerBand::updatePrice(double price)
{
	double ma = m_MA.updatePrice(price);
	m_Variance.updatePrice(price);
	return ma;
}

double BollingerBand::getMidValue()
{
	return m_MA.getAverage();
}

double BollingerBand::getUpperValue()
{
	return m_MA.getAverage() + (m_Scale*m_Variance.getStandardDeviation());
}

double BollingerBand::getLowerValue()
{
	return m_MA.getAverage() - (m_Scale*m_Variance.getStandardDeviation());
}
