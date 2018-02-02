#include "Helpers.h"
#include <fstream>

std::string Helpers::loadFile(const std::string &filename)
{
	std::ifstream infile(filename);
	std::string str((std::istreambuf_iterator<char>(infile)),
		std::istreambuf_iterator<char>());

	infile.close();
	return str;
}
