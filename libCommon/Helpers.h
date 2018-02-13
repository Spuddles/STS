#pragma once
#include <string>
#include <chrono>

class Helpers
{
public:
	static std::string	loadFile(const std::string &filename);
	static std::string	getCurrentTime();
	static uint64_t		getCurrentTimestamp();
	static bool			doesFileExist(const std::string &filename);
	static std::string	convertTime(const uint64_t &timestamp);

private:
	Helpers() {};
	virtual ~Helpers() {};
};

