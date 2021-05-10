#pragma once
#ifndef _LOG_UTIL_H_
#define _LOG_UTIL_H_

#include <iostream>
#include <string>

#ifndef _WIN64
#define LOG_RED "\033[31m"     /* Red */
#define LOG_WHITE "\033[37m"      /* White */

#else
#include <Windows.h>
#endif
class LogUtil {
public:
	static void printInfo(const std::string& str)
	{
#ifndef _WIN64
		std::cout << LOG_WHITE << "[INFO]" << str << std::endl;
#else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		std::cout << "[INFO]" << str << std::endl;
#endif
	}

	static void printError(const std::string& str)
	{
#ifndef _WIN64
		std::cout << LOG_RED << "[ERROR]" << str << std::endl;
#else
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "[ERROR]" << str << std::endl;
#endif
	}

};
#endif