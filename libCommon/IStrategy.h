#pragma once

namespace STS
{
	class Price;
	class IStrategy
	{
		virtual void	updatePrice(const Price &price) = 0;
		virtual bool	isBuySignal() = 0;
		virtual bool	isSellSignal() = 0;
	};
} // namespace STS