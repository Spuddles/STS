#pragma once
#include <queue>

namespace STS
{
	class MovingAverage
	{
	public:
		MovingAverage(int timePeriods);
		virtual ~MovingAverage();

		double	updatePrice(double price);
		double	getAverage() const;

	private:
		int					m_timePeriods;
		std::deque<double>	m_queuePrices;
		double				m_lastAverage;
	};
} // namespace STS