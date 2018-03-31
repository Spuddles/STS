#include "Gradient.h"

using namespace STS;

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
	m_MA.update(price);
}

bool Gradient::isGoingUp() const
{
	return (m_MA.getAverage() >= m_previousPrice);
}

bool Gradient::isGoingDown() const
{
	return (m_MA.getAverage() <= m_previousPrice);
}

double Gradient::getValue() const
{
	return m_MA.getAverage();
}
