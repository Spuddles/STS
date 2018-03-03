#pragma once

namespace STS
{
	class Price;
	class IStrategy
	{
		virtual void	updatePrice(const Price &price) = 0;
		virtual bool	isBuySignal() const = 0;
		virtual bool	isSellSignal() const = 0;
		virtual void	bought(const Price &price) = 0;
		virtual void	sold(const Price &price) = 0;
	};
} // namespace STS