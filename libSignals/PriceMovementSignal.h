#pragma once
#include "ISignal.h"
#include "MovingAverage.h"

namespace STS
{
	/*
	 * This signal creates a SMA of the price and returns
	 * the percentage difference of the current price and that MA
	 */
	class PriceMovementSignal : public ISignal
	{
	public:
				PriceMovementSignal(int periods);
		virtual ~PriceMovementSignal();

		void	update(const Price &price);
		double	getValue() const;
	private:
		MovingAverage	m_MA;
		double			m_lastPrice;
	};
} // namespace STS

