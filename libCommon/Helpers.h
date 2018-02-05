#pragma once
#include <string>
#include <chrono>

class Helpers
{
public:
	static std::string	loadFile(const std::string &filename);
	static std::string	getCurrentTime();

private:
	Helpers() {};
	virtual ~Helpers() {};
};

