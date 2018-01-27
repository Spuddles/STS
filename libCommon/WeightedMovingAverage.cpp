#include "WeightedMovingAverage.h"
#include <cassert>

WeightedMovingAverage::WeightedMovingAverage(double weight):
	m_Weight(weight),
	m_WMA(0.0)
{
	assert(weight > 0.0);
	assert(weight <= 1.0);
}

WeightedMovingAverage::~WeightedMovingAverage()
{
}

double WeightedMovingAverage::addPrice(double price)
{
	m_WMA = ((1.0 - m_Weight)*m_WMA) + (m_Weight * price);
	return m_WMA;
}

double WeightedMovingAverage::getAverage()
{
	return m_WMA;
}
