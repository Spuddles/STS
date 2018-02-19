#pragma once
#include <string>
#include <map>
#include <vector>

namespace STS
{
	struct Holding
	{
		std::string	m_Coin;
		double		m_Available;
		double		m_Locked;

		Holding(const std::string &coin, double &available, double &locked) :
			m_Coin(coin), m_Available(available), m_Locked(locked) {}
	};

	class AccountInfo
	{
	public:
		AccountInfo();
		virtual ~AccountInfo();

		void	insertHolding(const std::string &coin, double &available, double &locked);
		void	getAllHoldings(std::vector<Holding> &vecHoldings) const;
		void	getActiveHoldings(std::vector<Holding> &vecHoldings) const;
		bool	getHolding(const std::string &coin, Holding &holding) const;

		void	setCanTrade(bool bCanTrade);
		bool	canTrade() const;
		void	setCanWithdraw(bool bCanWithdraw);
		bool	canWithdraw() const;
		void	setCanDeposit(bool bCanDeposit);
		bool	canDeposit() const;

	private:
		std::vector<Holding>			m_vecHoldings;
		std::map<std::string, Holding>	m_mapHoldings;

		bool							m_bCanTrade;
		bool							m_bCanWithdraw;
		bool							m_bCanDeposit;
	};
} // namespace STS
