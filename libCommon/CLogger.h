#pragma once
#include <string>
#include <iostream>

class CLogger
{
	enum LogLevel
	{
		DEBUG = 0,
		NORM,
		WARN,
		ERROR,
		CRITICAL,
		TRADE,
		POSITION,
		SIGNAL
	};


public:
			CLogger();
	virtual ~CLogger();

	static void	Log(LogLevel level, const std::string &msg);
	static void	setLogLevel(LogLevel level);

private:
	static	LogLevel m_level;
	
	// Store the file handles for the different log types
	ostream
};

