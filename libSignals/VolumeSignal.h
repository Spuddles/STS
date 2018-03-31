#pragma once
#include "ISignal.h"
#include "MovingAverage.h"

namespace STS
{
	/*
	 * This signal keeps a SMA of the volume and returns the percentage
	 * difference between the last volume and the SMA value.
	 */
	class VolumeSignal : public ISignal
	{
	public:
				VolumeSignal(int periods);
		virtual ~VolumeSignal();

		void	update(const Price &price);
		double	getValue() const;
	private:
		MovingAverage	m_MA;
		double			m_lastPrice;
	};
} // namespace STS

