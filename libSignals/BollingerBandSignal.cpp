#include "BollingerBandSignal.h"
#include "Price.h"

using namespace STS;

BollingerBandSignal::BollingerBandSignal(int periods, double scale) :
	m_BB(periods, scale)
{
}

BollingerBandSignal::~BollingerBandSignal()
{
}

void BollingerBandSignal::update(const Price &price)
{
	m_BB.updatePrice(price.getClose());
}

double BollingerBandSignal::getValue() const
{
	double result = 0.0;
	return result;
}
