#pragma once
#include "MovingAverage.h"

class Gradient
{
public:
			Gradient(int timePeriods);
	virtual ~Gradient();

	void	updatePrice(double price);
	bool	isGoingUp();
	bool	isGoingDown();

private:
	MovingAverage	m_MA;
	double			m_previousPrice;
};

