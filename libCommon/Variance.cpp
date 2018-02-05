#include "Variance.h"
#include <cmath>

Variance::Variance(int timePoints):
	m_timePoints(timePoints)
{
}

Variance::~Variance()
{
}

double Variance::updatePrice(double price)
{
	m_queuePrices.push_front(price);
	if (m_queuePrices.size() > m_timePoints)
	{
		m_queuePrices.pop_back();
	}

	double average = 0.0;
	for (double p : m_queuePrices)
	{
		average += p;
	}
	average /= m_queuePrices.size();

	m_Variance = 0.0;
	for (double p : m_queuePrices)
	{
		m_Variance += (p-average)*(p-average);
	}
	
	if (m_queuePrices.size() > 1)
	{
		m_Variance /= m_queuePrices.size() - 1;
	}
	return m_Variance;
}

double Variance::getVariance()
{
	return m_Variance;
}

double Variance::getStandardDeviation()
{
	return sqrt(m_Variance);
}
