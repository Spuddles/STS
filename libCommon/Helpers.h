#pragma once
#include <string>

class Helpers
{
public:
	static std::string	loadFile(const std::string &filename);

private:
	Helpers() {};
	virtual ~Helpers() {};
};

