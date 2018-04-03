#pragma once
#include <string>
#include "IStrategy.h"

namespace STS
{
	class DynamicLoader
	{
	public:
		DynamicLoader();
		virtual ~DynamicLoader();

		static IStrategy* loadModule(const std::string &name);
	};
} // namespace STS
