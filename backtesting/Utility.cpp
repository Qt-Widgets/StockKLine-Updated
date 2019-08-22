#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <chrono>
#include <ctime>
#include "date.h"

std::string CurrentDate()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char buf[10] = { 0 };
	std::strftime(buf, sizeof(buf), "%Y%m%d", std::localtime(&now));
	return buf;
}

std::string getDate(int days)
{
	std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point new_t = t + days * std::chrono::hours(24);

	std::time_t now = std::chrono::system_clock::to_time_t(new_t);

	char buf[10] = { 0 };
	std::strftime(buf, sizeof(buf), "%Y%m%d", std::localtime(&now));
	return buf;
}

std::string CurrentDateTime()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char buf[30] = { 0 };
	std::strftime(buf, sizeof(buf), "%Y%m%d %H:%M:%S", std::localtime(&now));
	return buf;
}

std::string CurrentTime()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	char buf[30] = { 0 };
	std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
	return buf;
}

std::string CurrentDateTimeWithMilli()
{
	return date::format("%FT%TZ", std::chrono::system_clock::now());
}

std::string getInstrumentPrefix(char *instrumentId)
{
	if (!instrumentId) return "";
	int c = 0;
	//for (int i = 0; !(instrumentId[i] >= '0' && instrumentId[i] <= '9'); i++)
	// it seems there is some special hidden characters in some ids, so have to filter them explicitly
	for (int i = 0;
			(instrumentId[i] >= 'a' && instrumentId[i] <= 'z') || 
			(instrumentId[i] >= 'A' && instrumentId[i] <= 'Z') ||
			instrumentId[i] == '_';
		i++) {
		c++;
	}
	std::string prefix(instrumentId, c);
	return prefix;
}

int getMinutesOfDay(const char* dateTimeStr)
{
	if (!dateTimeStr) {
		return 0;
	}

	int tmp, hour, minute;
	sscanf(dateTimeStr, "%4d-%2d-%2d %2d:%2d", &tmp, &tmp, &tmp, &hour, &minute);
	return hour * 60 + minute;
}
