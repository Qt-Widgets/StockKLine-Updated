#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <cmath>
#include <limits>
#include <iostream>
#include <type_traits>

// Test whether two float or double numbers are equal.
// ulp: units in the last place.
template <typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
isAlmostEqual(T x, T y, int ulp = 2)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::fabs(x - y) < std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
		// unless the result is subnormal
		|| std::fabs(x - y) < std::numeric_limits<T>::min();
}

std::string CurrentDate();

std::string getDate(int days);

std::string CurrentDateTime();

std::string CurrentTime();

std::string CurrentDateTimeWithMilli();

std::string getInstrumentPrefix(char *instrumentId);

int getMinutesOfDay(const char* dateTimeStr);

#endif