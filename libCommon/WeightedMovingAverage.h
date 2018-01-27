#pragma once
class WeightedMovingAverage
{
public:
			WeightedMovingAverage(double weight);
	virtual ~WeightedMovingAverage();

	double	addPrice(double price);
	double	getAverage();

private:

	double	m_Weight;
	double	m_WMA;
};

