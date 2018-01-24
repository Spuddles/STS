#pragma once
#include <string>

class Product
{
public:
				Product();
	virtual		~Product();

	uint32_t	getID() const { return m_id; }
	void		setID(uint32_t id) { m_id = id; }

	bool		isActive() const { return m_active; }
	void		setActive(bool active) { m_active = active; }

	std::string	getBaseAsset() const { return m_baseAsset; }
	void		setBaseAsset(std::string asset) { m_baseAsset = asset; }

	std::string	getBaseAssetName() const { return m_baseAssetName; }
	void		setBaseAssetName(std::string name) { m_baseAssetName = name; }

	std::string	getBaseAssetUnit() const { return m_baseAssetUnit; }
	void		setBaseAssetUnit(std::string unit) { m_baseAssetUnit = unit; }
	
	uint8_t		getDecimalPlaces() const { return m_decimalPlaces; }
	void		setDecimalPlaces(uint8_t decimalPlaces) { m_decimalPlaces = decimalPlaces; }
	
	std::string	getMatchingUnitType() const { return m_matchingUnitType; }
	void		setMatchingUnitType(std::string unitType) { m_matchingUnitType = unitType; }
	
	double		getMinQty() const { return m_minQty; }
	void		setMinQty(double minQty) { m_minQty = minQty; }
	
	double		getMinTrade() const { return m_minTrade; }
	void		setMinTrade(double minTrade) { m_minTrade = minTrade; }
	
	std::string	getQuoteAsset() const { return m_quoteAsset; }
	void		setQuoteAsset(std::string asset) { m_quoteAsset = asset; }
	
	std::string getQuoteAssetName() const { return m_quoteAssetName; }
	void		setQuoteAssetName(std::string name) { m_quoteAssetName = name; }
	
	std::string getQuoteAssetUnit() const { return m_quoteAssetUnit; }
	void		setQuoteAssetUnit(std::string unit) { m_quoteAssetUnit = unit; }
	
	std::string	getStatus() const { return m_status; }
	void		setStatus(std::string status) { m_status = status; }
	
	std::string getSymbol() const { return m_symbol; }
	void		setSymbol(std::string symbol) { m_symbol = symbol; }
	
	double		getTickSize() const { return m_tickSize; }
	void		setTickSize(double tickSize) { m_tickSize = tickSize; }
	
	double		getWithdrawFee() const { return m_withdrawFee; }
	void		setWithdrawFee(double withdrawFee) { m_withdrawFee = withdrawFee; }

private:
	uint32_t	m_id;
	bool		m_active;
	std::string	m_baseAsset;
	std::string m_baseAssetName;
	std::string m_baseAssetUnit;
	uint8_t		m_decimalPlaces;
	std::string m_matchingUnitType;
	double		m_minQty;
	double		m_minTrade;
	std::string	m_quoteAsset;
	std::string m_quoteAssetName;
	std::string m_quoteAssetUnit;
	std::string	m_status;
	std::string m_symbol;
	double		m_tickSize;
	double		m_withdrawFee;
};

