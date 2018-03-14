#pragma once
#include <string>
#include <chrono>

namespace STS
{
	class Helpers
	{
	public:
		static std::string	loadFile(const std::string &filename);
		static std::string	getCurrentTime();
		static uint64_t		getCurrentTimestamp();
		static void			getCurrentTime(int &hours, int &minutes, int &seconds);
		static bool			doesFileExist(const std::string &filename);
		static std::string	convertTime(const uint64_t &timestamp);
		static void			sleepMS(int milliseconds);
		static uint64_t		convertDateToTimestamp(const std::string &date);

	private:
		Helpers() {};
		virtual ~Helpers() {};
	};
} // namespace STS