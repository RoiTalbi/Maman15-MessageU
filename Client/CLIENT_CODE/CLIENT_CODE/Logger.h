#pragma once



/* Includes */
#include <string>
#include <iostream>



/* Constants and Macros */


class Logger
{

public:
	static void LOG_INFO(const std::string& message)
	{
		std::cout << "|LOG INFO| \t " << message;
	}


	static void LOG_WARNING(const std::string& message)
	{
		std::cout << "|LOG WARNING| \t " << message;
	}


	static void LOG_ERROR(const std::string& message)
	{
		std::cout << "|LOG ERROR| \t " << message;
	}


	/* Errors that cannot allow continue with process's run*/
	static void LOG_CRITICAL_ERROR(const std::string& message)
	{
		std::cerr << "|CRITICAL ERROR!!| \t " << message;
	}

};