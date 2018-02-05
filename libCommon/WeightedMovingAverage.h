#pragma once
class WeightedMovingAverage
{
public:
			WeightedMovingAverage(double weight);
	virtual ~WeightedMovingAverage();

	double	updatePrice(double price);
	double	getAverage();

private:
	double	m_Weight;
	double	m_WMA;
	bool	m_bFirst;
};

