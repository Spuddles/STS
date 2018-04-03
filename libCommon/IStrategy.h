#pragma once

namespace STS
{
	class Price;
	class Params;
	class IStrategy
	{
	public:
		virtual void	initialise(const Params &p) = 0;
		virtual void	updatePrice(const Price &price) = 0;
		virtual bool	isBuySignal() const = 0;
		virtual bool	isSellSignal() const = 0;
		virtual void	bought(const Price &price) = 0;
		virtual void	sold(const Price &price) = 0;

		virtual void	resetBuySell() = 0;
	};
} // namespace STS