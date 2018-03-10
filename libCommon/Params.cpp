#include "Params.h"
#include "json.hpp"
#include "Helpers.h"
#include <string>
#include <iostream>

using namespace STS;

using json = nlohmann::json;

Params::Params()
{
}

Params::~Params()
{
}

bool Params::load(const std::string &filename)
{
	std::string config = Helpers::loadFile(filename);
	json j = json::parse(config);

	// Iterate through the collection and add to the lookup maps
	for (json::iterator it = j.begin(); it != j.end(); it++)
	{
		json &item = (*it);

		std::string name = it.key();
		std::cout << item.dump() << std::endl;

		if (item.is_number_float())
		{
			double	value = item;
			m_mapDoubles.insert(std::pair<std::string, double>(name, value));
		}
		else if (item.is_number_integer())
		{
			int value = item;
			m_mapInts.insert(std::pair<std::string, int>(name, value));
		}
		else if (item.is_string())
		{
			std::string value = item;
			m_mapStrings.insert(std::pair<std::string, std::string>(name, value));
		}
		else if (item.is_object())
		{
			// It's a range, handle it
			std::cout << "Range: " << item.dump() << std::endl;


			if (item.find("start") == item.end() ||
				item.find("end") == item.end() ||
				item.find("step") == item.end())
			{
				std::cout << "Missing parameter in the range set" << std::endl;
			}

			if (item["start"].is_number() ||
				item["end"].is_number())
			{
				RangeParam<double> rp(item["start"], item["end"], item["step"]);

				m_vecRanges.push_back(rp);
				m_mapDoubleRanges.insert(std::pair<std::string, RangeParam<double>>(name, rp));
			}
		}
	}
	return true;
}

bool Params::getInt(const std::string &name, int &value) const
{
	auto it = m_mapInts.find(name);
	if (it != m_mapInts.end())
	{
		value = it->second;
		return true;
	}

	auto it2 = m_mapDoubleRanges.find(name);
	if (it2 != m_mapDoubleRanges.end())
	{
		RangeParam<double> rp = it2->second;
		value = static_cast<int>(rp.get());
		return true;
	}
	return false;
}

bool Params::getDouble(const std::string &name, double &value) const
{
	auto it = m_mapDoubles.find(name);
	if (it != m_mapDoubles.end())
	{
		value = it->second;
		return true;
	}

	auto it2 = m_mapDoubleRanges.find(name);
	if (it2 != m_mapDoubleRanges.end())
	{
		const RangeParam<double> rp = it2->second;
		value = rp.get();
		return true;
	}
	return false;
}

bool Params::getString(const std::string &name, std::string &value) const
{
	auto it = m_mapStrings.find(name);
	if (it != m_mapStrings.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

size_t Params::getRangeCount() const
{
	size_t count = 1;
	for (auto r : m_vecRanges)
	{
		count *= r.getStepCount();
	}
	return count;
}

bool Params::hasRanges() const
{
	return (!m_mapDoubleRanges.empty());
}

