#pragma once
#include <queue>

namespace STS
{
	class MovingAverage
	{
	public:
				MovingAverage(int timePeriods);
		virtual ~MovingAverage();

		double	update(double value);
		double	getAverage() const;

	private:
		int					m_timePeriods;
		std::deque<double>	m_queueValues;
		double				m_lastAverage;
	};
} // namespace STS