#include "VolumeSignal.h"
#include "Price.h"

using namespace STS;

VolumeSignal::VolumeSignal(int periods) :
	m_MA(periods)
{
}

VolumeSignal::~VolumeSignal()
{
}

void VolumeSignal::update(const Price &price)
{
	m_MA.update(price.getVolume());
	m_lastPrice = price.getClose();
}

double VolumeSignal::getValue() const
{
	double result = (m_lastPrice - m_MA.getAverage()) / m_MA.getAverage();
	return result;
}
