/*
 * Copyright (C) 2006-2007  Wengo
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 *
 * Redistribution and use is allowed according to the terms of the BSD license.
 * For details see the accompanying COPYING file.
 */

#include <string>
#include <ctime>
#include <iostream>
#include <sstream>

#include <cstdlib>

/**
 * Transforms an integer to a std::string.
 */
std::string fromNumber(int number) {
	std::stringstream ss;
	ss << number;
	return ss.str();
}

/**
 * Gets current date time.
 *
 * Example: 20070323112132
 * 2007-03-23 11:21:32 (YYYY-MM-DD HH:MM:SS)
 */
std::string getCurrentDateTime() {
	std::time_t currentTime = time(NULL);
	struct std::tm * timeInfo = std::localtime(&currentTime);

	std::string month = fromNumber(timeInfo->tm_mon + 1);
	std::string day = fromNumber(timeInfo->tm_mday);
	std::string year = fromNumber(timeInfo->tm_year + 1900);

	std::string hour = fromNumber(timeInfo->tm_hour);
	std::string minute = fromNumber(timeInfo->tm_min);
	std::string second = fromNumber(timeInfo->tm_sec);

	if (month.size() == 1) {
		month = "0" + month;
	}

	if (day.size() == 1) {
		day = "0" + day;
	}

	if (hour.size() == 1) {
		hour = "0" + hour;
	}

	if (minute.size() == 1) {
		minute = "0" + minute;
	}

	if (second.size() == 1) {
		second = "0" + second;
	}

	return year + month + day + hour + minute + second;
}

int main(void) {
	std::cout << getCurrentDateTime() << std::endl;

	return EXIT_SUCCESS;
}
