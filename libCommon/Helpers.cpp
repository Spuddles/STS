#include "Helpers.h"
#include <fstream>
#include <ctime>

std::string Helpers::loadFile(const std::string &filename)
{
	std::ifstream infile(filename);
	std::string str((std::istreambuf_iterator<char>(infile)),
		std::istreambuf_iterator<char>());

	infile.close();
	return str;
}

std::string Helpers::getCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &tt);

	char	buffer[14];
	sprintf_s(&buffer[0], 14, "%02d:%02d:%02d", local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);

	return buffer;
}

