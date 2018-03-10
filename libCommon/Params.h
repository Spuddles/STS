#pragma once
#include <map>
#include <vector>
#include "RangeParam.h"

namespace STS
{
	class Params
	{
	public:
		Params();
		virtual ~Params();

		bool	load(const std::string &filename);
		bool	getInt(const std::string &name, int &value) const;
		bool	getDouble(const std::string &name, double &value) const;
		bool	getString(const std::string &name, std::string &value) const;

		size_t	getRangeCount() const;
		bool	hasRanges() const;

	private:
		std::map<std::string, int>					m_mapInts;
		std::map<std::string, double>				m_mapDoubles;
		std::map<std::string, RangeParam<double>>	m_mapDoubleRanges;
		std::map<std::string, std::string>			m_mapStrings;

		std::vector<RangeParam<double>>				m_vecRanges;
	};
} // namespace STS