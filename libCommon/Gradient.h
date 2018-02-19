#pragma once
#include "MovingAverage.h"

namespace STS
{
	class Gradient
	{
	public:
		Gradient(int timePeriods);
		virtual ~Gradient();

		void	updatePrice(double price);
		bool	isGoingUp() const;
		bool	isGoingDown() const;
		double	getValue() const;

	private:
		MovingAverage	m_MA;
		double			m_previousPrice;
	};
} // namespace STS