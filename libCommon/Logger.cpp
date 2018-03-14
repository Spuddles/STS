#include "Logger.h"
#include "Helpers.h"
#include <sstream>

using namespace STS;

Logger::LogLevel Logger::m_logLevel = LogLevel::NORM;
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

std::string Logger::getLogLevelStr(LogLevel level)
{
	switch (level)
	{
	case	DEBUG:		return "DEBUG";
	case	NORM:		return "NORMAL";
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