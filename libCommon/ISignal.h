#pragma once

namespace STS
{
	class Price;

	class ISignal
	{
		virtual void	update(const Price &price) = 0;
		virtual double	getValue() = 0;
	};
} // namespace STS
