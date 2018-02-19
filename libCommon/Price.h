#pragma once
#include <stdint.h>

namespace STS
{
	class Price
	{
	public:
		Price();
		virtual		~Price();

		unsigned int	getProductID() const { return m_ProductID; }
		void			setProductID(unsigned int id) { m_ProductID = id; }

		uint64_t	getOpenTime() const { return m_openTime; }
		void		setOpenTime(uint64_t time) { m_openTime = time; }

		double		getOpen() const { return m_open; }
		void		setOpen(double open) { m_open = open; }

		double		getHigh() const { return m_high; }
		void		setHigh(double high) { m_high = high; }

		double		getLow() const { return m_low; }
		void		setLow(double low) { m_low = low; }

		double		getClose() const { return m_close; }
		void		setClose(double close) { m_close = close; }

		double		getVolume() const { return m_volume; }
		void		setVolume(double volume) { m_volume = volume; }

		uint64_t	getCloseTime() const { return m_closeTime; }
		void		setCloseTime(uint64_t time) { m_closeTime = time; }

		uint64_t	getTrades() const { return m_trades; }
		void		setTrades(uint64_t trades) { m_trades = trades; }

		double		getMid() const { return (m_open + m_close) / 2.0; }

	private:
		unsigned int m_ProductID;
		uint64_t	m_openTime;
		double		m_open;
		double		m_high;
		double		m_low;
		double		m_close;
		double		m_volume;
		uint64_t	m_closeTime;
		uint64_t	m_trades;
	};
} // namespace STS