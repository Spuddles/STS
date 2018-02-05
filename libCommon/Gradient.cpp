#include "Gradient.h"

Gradient::Gradient(int timePeriods):
	m_MA(timePeriods),
	m_previousPrice(0.0)
{
}

Gradient::~Gradient()
{
}

void Gradient::updatePrice(double price)
{
	m_previousPrice = m_MA.getAverage();
	m_MA.updatePrice(price);
}

bool Gradient::isGoingUp()
{
	return (m_MA.getAverage() >= m_previousPrice);
}

bool Gradient::isGoingDown()
{
	return (m_MA.getAverage() <= m_previousPrice);
}

