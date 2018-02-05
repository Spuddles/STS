#pragma once
#include <queue>

class Variance
{
public:
			Variance(int timePoints);
	virtual ~Variance();

	double	updatePrice(double price);
	double	getVariance();
	double	getStandardDeviation();

private:
	int					m_timePoints;
	std::deque<double>	m_queuePrices;
	double				m_Variance;
};

