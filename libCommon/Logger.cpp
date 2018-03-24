#include "Logger.h"
#include "Helpers.h"
#include <sstream>

using namespace STS;

Logger::LogLevel Logger::m_logLevel = LogLevel::NORMAL;
std::ofstream	 Logger::m_defaultLogFile;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::setDefaultLog(const std::string &filename)
{
	if (m_defaultLogFile.is_open())
	{
		m_defaultLogFile.close();
	}
	m_defaultLogFile.open(filename, std::ios::out);
}

void Logger::close()
{
	if (m_defaultLogFile.is_open())
		m_defaultLogFile.close();
}

void Logger::log(LogLevel level, const std::string &msg)
{
	if (level & m_logLevel)
	{
		std::stringstream ss;

		ss << Helpers::getCurrentTime() << ", ";
		ss << getLogLevelStr(level) << ", ";
		ss << msg << std::endl;

		if (m_defaultLogFile.is_open())
		{
			m_defaultLogFile << ss.str();
			return;
		}
		std::cout << ss.str();
	}
}

void Logger::setLogLevel(int level)
{
	m_logLevel = static_cast<LogLevel>(level);
}

void Logger::setLogLevel(const std::string &level)
{
	m_logLevel = NONE;

	size_t start = 0;
	size_t pos = 0;
	std::string temp;
	while (pos != std::string::npos)
	{
		pos = level.find("|", start);
		if (pos == std::string::npos)
		{
			temp = level.substr(start);
			m_logLevel = static_cast<LogLevel>(m_logLevel | getLogLevelFromStr(temp));
		}
		else
		{
			temp = level.substr(start, (pos-start));
			m_logLevel = static_cast<LogLevel>(m_logLevel | getLogLevelFromStr(temp));
			start = pos + 1;
		}
	}
}

void Logger::clearLogMask()
{
	m_logLevel = static_cast<LogLevel>(0);
}

void Logger::addLogMask(LogLevel level)
{
	m_logLevel = static_cast<LogLevel>(m_logLevel | level);
}

void Logger::fillLogMask()
{
	m_logLevel = static_cast<LogLevel>(-1);
}

std::string Logger::getLogLevelStr(Logger::LogLevel level)
{
	switch (level)
	{
	case	DEBUG:		return "DEBUG";
	case	NORMAL:		return "NORMAL";
	case	WARN:		return "WARN";
	case	ERROR:		return "ERROR";
	case	CRITICAL:	return "CRITCIAL";
	case	TRADE:		return "TRADE";
	case	POSITION:	return "POSITION";
	case	SIGNAL:		return "SIGNAL";
	case	ALGO:		return "ALGO";
	case	EXCHANGE:	return "EXCHANGE";
	case	PNL:		return "PNL";
	default: return "UNKNOWN";
	}
}

Logger::LogLevel Logger::getLogLevelFromStr(const std::string &logLevel)
{
	if (logLevel.compare("DEBUG") == 0)	return DEBUG;
	else if (logLevel.compare("NORMAL") == 0)	return NORMAL;
	else if (logLevel.compare("WARN") == 0)	return WARN;
	else if (logLevel.compare("ERROR") == 0)	return ERROR;
	else if (logLevel.compare("CRITICAL") == 0)	return CRITICAL;
	else if (logLevel.compare("TRADE") == 0)	return TRADE;
	else if (logLevel.compare("POSITION") == 0)	return POSITION;
	else if (logLevel.compare("SIGNAL") == 0)	return SIGNAL;
	else if (logLevel.compare("ALGO") == 0)	return ALGO;
	else if (logLevel.compare("EXCHANGE") == 0)	return EXCHANGE;
	else if (logLevel.compare("PNL") == 0)	return PNL;
	return NONE;
}