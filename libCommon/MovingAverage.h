#pragma once
#include <queue>

class MovingAverage
{
public:
			MovingAverage(int timePeriods);
	virtual ~MovingAverage();

	double	addPrice(double price);
	double	getAverage();

private:
	int					m_timePeriods;
	std::deque<double>	m_queuePrices;
	double				m_lastAverage;
};

