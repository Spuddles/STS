#pragma once
#include "ISignal.h"
#include "BollingerBand.h"

namespace STS
{
	/*
	 * This signal 
	 * 
	 */
	class BollingerBandSignal : public ISignal
	{
	public:
				BollingerBandSignal(int periods, double scale);
		virtual ~BollingerBandSignal();

		void	update(const Price &price);
		double	getValue() const;
	private:
		BollingerBand	m_BB;
		double			m_lastPrice;
	};
} // namespace STS

