#include "MovingAverage.h"



MovingAverage::MovingAverage(int timePeriods) :
	m_timePeriods(timePeriods),
	m_lastAverage(0.0)
{
}

MovingAverage::~MovingAverage()
{
}

double MovingAverage::updatePrice(double price)
{
	m_queuePrices.push_front(price);

	if (m_queuePrices.size() > m_timePeriods)
	{
		m_queuePrices.pop_back();
	}

	double average = 0.0;
	for (double p : m_queuePrices)
	{
		average += p;
	}
	m_lastAverage = average / m_queuePrices.size();
	return m_lastAverage;
}

double MovingAverage::getAverage()
{
	return m_lastAverage;
}

