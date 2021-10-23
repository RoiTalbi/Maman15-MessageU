#pragma once



/* Includes */
#include <string>
#include <iostream>



/* Constants and Macros */


class Logger
{

public:
	static void LOG_INFO(const std::string& message, const std::string& details = "")
	{
		std::cout << "|INFO| \t " << message << ".  " << details << std::endl;
	}


	static void LOG_WARNING(const std::string& message, const std::string& details = "")
	{
		std::cout << "|WARNING| \t " << message << ".  " << details << std::endl;
	}


	static void LOG_ERROR(const std::string& message, const std::string& details = "")
	{
		std::cout << "|ERROR| \t " << message << ".  " << details << std::endl;
	}


	/* Errors that cannot allow continue with process's run*/
	static void LOG_CRITICAL_ERROR(const std::string& message, const std::string& details = "")
	{
		std::cerr << "|CRITICAL ERROR!| \t " << message << ".  " << details << std::endl;
	}

};