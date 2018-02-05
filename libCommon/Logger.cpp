#include "Logger.h"
#include "Helpers.h"
#include <sstream>

Logger::LogLevel Logger::m_logLevel = LogLevel::NORM;
std::ofstream	 Logger::m_defaultLogFile;
std::ofstream	 Logger::m_tradeLogFile;
std::ofstream	 Logger::m_positionLogFile;
std::ofstream	 Logger::m_signalLogFile;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::setDefaultLog(const std::string &filename)
{
	m_defaultLogFile.open(filename, std::ios::out);
}

void Logger::close()
{
	if (m_defaultLogFile.is_open())
		m_defaultLogFile.close();

	if (m_tradeLogFile.is_open())
		m_tradeLogFile.close();

	if (m_positionLogFile.is_open())
		m_positionLogFile.close();

	if (m_signalLogFile.is_open())
		m_signalLogFile.close();
}

void Logger::log(LogLevel level, const std::string &msg)
{
	std::stringstream ss;

	ss << Helpers::getCurrentTime() << ", ";
	ss << getLogLevelStr(level) << ", ";
	ss << msg << std::endl;

	if (level == LogLevel::SIGNAL && m_signalLogFile.is_open())
	{
		m_signalLogFile << ss.str();
		return;
	}
	else if (level == LogLevel::POSITION && m_signalLogFile.is_open())
	{
		m_positionLogFile << ss.str();
		return;
	}
	else if (level == LogLevel::TRADE && m_signalLogFile.is_open())
	{
		m_tradeLogFile << ss.str();
		return;
	}
	m_defaultLogFile << ss.str();
}

void Logger::setLogLevel(LogLevel level)
{

}

void Logger::setTradeLog(const std::string &filename)
{

}

void Logger::setPositionLog(const std::string &filename)
{

}

void Logger::setSignalLog(const std::string &filename)
{

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
	default: return "UNKNOWN";
	}
}
