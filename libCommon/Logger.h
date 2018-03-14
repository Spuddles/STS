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

		enum LogLevel : unsigned int
		{
			DEBUG		= 1,
			NORM		= 2,
			WARN		= 4,
			ERROR		= 8,
			CRITICAL	= 16,
			TRADE		= 32,
			POSITION	= 64,
			SIGNAL		= 128,
			ALGO		= 256,
			EXCHANGE	= 512,
			PNL			= 1024
		};

		Logger();
		virtual ~Logger();
		static void	close();

		static void setDefaultLog(const std::string &filename);
		static void	log(LogLevel level, const std::string &msg);
		static void	setLogLevel(int level);
		static void clearLogMask();
		static void addLogMask(LogLevel level);
		static void fillLogMask();

	private:
		static std::string	getLogLevelStr(LogLevel level);

		static LogLevel		m_logLevel;

		// Store the file handles for the different log types
		static std::ofstream	m_defaultLogFile;
	};
} // namespace STS