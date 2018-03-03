#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace STS
{
	#define Log(l,m) Logger::log(Logger::LogLevel::l, m)

	class Logger
	{
	public:

		enum LogLevel
		{
			DEBUG = 0,
			NORM,
			WARN,
			ERROR,
			CRITICAL,
			TRADE,
			POSITION,
			SIGNAL,
			ALGO,
			EXCHANGE
		};

		Logger();
		virtual ~Logger();
		static void	close();

		static void setDefaultLog(const std::string &filename);
		static void	log(LogLevel level, const std::string &msg);
		static void	setLogLevel(LogLevel level);
		static void setTradeLog(const std::string &filename);
		static void setPositionLog(const std::string &filename);
		static void setSignalLog(const std::string &filename);

	private:
		static std::string	getLogLevelStr(LogLevel level);

		static LogLevel		m_logLevel;

		// Store the file handles for the different log types
		static std::ofstream	m_defaultLogFile;
		static std::ofstream	m_tradeLogFile;
		static std::ofstream	m_positionLogFile;
		static std::ofstream	m_signalLogFile;
	};
} // namespace STS