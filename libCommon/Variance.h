#pragma once
#include <queue>

namespace STS
{
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
} // namespace STS