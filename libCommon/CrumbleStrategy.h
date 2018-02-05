#pragma once
#include "IStrategy.h"
#include "BollingerBand.h"
#include "Gradient.h"

class CrumbleStrategy : public IStrategy
{
public:
			CrumbleStrategy();
	virtual ~CrumbleStrategy();

	void	updatePrice(const Price &price);
	bool	isBuySignal();
	bool	isSellSignal();

private:
	BollingerBand	m_BB;
	Gradient		m_Gradient;
	int				m_BuySignalCount;
	int				m_SellSignalCount;
};

