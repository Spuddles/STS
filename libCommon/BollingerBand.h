#pragma once
#include "MovingAverage.h"
#include "Variance.h"

class BollingerBand
{
public:
			BollingerBand(int timePeriods, double scale);
	virtual ~BollingerBand();

	double	addPrice(double price);
	double	getMidValue();
	double	getUpperValue();
	double	getLowerValue();

private:
	double			m_Scale;
	MovingAverage	m_MA;
	Variance		m_Variance;
};

