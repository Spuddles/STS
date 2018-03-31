#include "MovingAverage.h"

using namespace STS;

MovingAverage::MovingAverage(int timePeriods) :
	m_timePeriods(timePeriods),
	m_lastAverage(0.0)
{
}

MovingAverage::~MovingAverage()
{
}

double MovingAverage::update(double value)
{
	m_queueValues.push_front(value);

	if (m_queueValues.size() > m_timePeriods)
	{
		m_queueValues.pop_back();
	}

	double average = 0.0;
	for (double p : m_queueValues)
	{
		average += p;
	}
	m_lastAverage = average / m_queueValues.size();
	return m_lastAverage;
}

double MovingAverage::getAverage() const
{
	return m_lastAverage;
}

