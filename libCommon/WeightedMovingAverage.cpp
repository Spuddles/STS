#include "WeightedMovingAverage.h"
#include <cassert>

using namespace STS;

WeightedMovingAverage::WeightedMovingAverage(double weight) :
	m_Weight(weight),
	m_WMA(0.0),
	m_bFirst(true)
{
	assert(weight > 0.0);
	assert(weight <= 1.0);
}

WeightedMovingAverage::~WeightedMovingAverage()
{
}

double WeightedMovingAverage::updatePrice(double price)
{
	if (m_bFirst)
	{
		m_WMA = price;
		m_bFirst = false;
	}
	else
	{
		m_WMA = ((1.0 - m_Weight)*m_WMA) + (m_Weight * price);
	}
	return m_WMA;
}

double WeightedMovingAverage::getAverage()
{
	return m_WMA;
}
