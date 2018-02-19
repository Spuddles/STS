#include "Helpers.h"
#include <fstream>
#include <ctime>
#include <thread>
#include <chrono>

using namespace STS;

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

void Helpers::getCurrentTime(int &hours, int &minutes, int &seconds)
{
	auto now = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(now);
	tm local_tm;
	localtime_s(&local_tm, &tt);

	hours = local_tm.tm_hour;
	minutes = local_tm.tm_min;
	seconds = local_tm.tm_sec;
}

uint64_t Helpers::getCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	time_t tt;
	tt = std::chrono::system_clock::to_time_t(now);
	uint64_t ts = tt * 1000;
	return ts;
}

bool Helpers::doesFileExist(const std::string &filename)
{
	std::ifstream f(filename.c_str());
	return f.good();
}

std::string Helpers::convertTime(const uint64_t &timestamp)
{
	int milliseconds = timestamp % 1000;
	time_t tt = timestamp / 1000;
	struct tm ts;
	localtime_s(&ts, &tt);

	char buffer[64];
	sprintf_s(buffer, 64, "%04d%02d%02d-%02d:%02d:%02d.%04d", ts.tm_year + 1900, ts.tm_mon + 1, ts.tm_mday, ts.tm_hour, ts.tm_min, ts.tm_sec, milliseconds);

	return std::string(buffer);
}

void Helpers::sleepMS(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
