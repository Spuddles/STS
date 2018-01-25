#pragma once
#include <stdint.h>

class Price
{
public:
				Price();
	virtual		~Price();

	uint64_t	getOpenTime() { return m_openTime; }
	void		setOpenTime(uint64_t time) { m_openTime = time; }

	double		getOpen() { return m_open; }
	void		setOpen(double open) { m_open = open; }

	double		getHigh() { return m_high; }
	void		setHigh(double high) { m_high = high; }

	double		getLow() { return m_low; }
	void		setLow(double low) { m_low = low; }

	double		getClose() { return m_close; }
	void		setClose(double close) { m_close = close; }

	double		getVolume() { return m_volume; }
	void		setVolume(double volume) { m_volume = volume; }

	uint64_t	getCloseTime() { return m_closeTime; }
	void		setCloseTime(uint64_t time) { m_closeTime = time; }

	uint64_t	getTrades() { return m_trades; }
	void		setTrades(uint64_t trades) { m_trades = trades; }

private:
	uint64_t	m_openTime;
	double		m_open;
	double		m_high;
	double		m_low;
	double		m_close;
	double		m_volume;
	uint64_t	m_closeTime;
	uint64_t	m_trades;
};

