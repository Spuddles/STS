#include "PriceMovementSignal.h"
#include "Price.h"

using namespace STS;

PriceMovementSignal::PriceMovementSignal(int periods) :
	m_MA(periods)
{
}

PriceMovementSignal::~PriceMovementSignal()
{
}

void PriceMovementSignal::update(const Price &price)
{
	m_MA.update(price.getClose());
	m_lastPrice = price.getClose();
}

double PriceMovementSignal::getValue() const
{
	double result = (m_lastPrice - m_MA.getAverage()) / m_MA.getAverage();
	return result;
}
