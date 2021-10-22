#pragma once

/* Includes */
#include <exception>
#include <string>



/* Error Codes */

#define CLIENT_GENERAL_ERROR_EXIT_CODE		(1)




/* Module Exceptions */

class ServerGeneralError : public std::exception
{
public:

	ServerGeneralError(const char* message) : _msg(message) {}

	ServerGeneralError(const std::string& message) : _msg(message) {}

	virtual ~ServerGeneralError() noexcept {}


	virtual const char* what() const 
	{
		return _msg.c_str();
	}

protected:

	std::string _msg;
};





class ClientNetworkError : public std::exception
{
public:
	/** Constructor (C strings).
	 *  @param message C-style string error message.
	 *                 The string contents are copied upon construction.
	 *                 Hence, responsibility for deleting the char* lies
	 *                 with the caller.
	 */
	ClientNetworkError(const char* message) : _msg(message) {}

	ClientNetworkError(const std::string& message) : _msg(message) {}

	virtual ~ClientNetworkError() noexcept {}


	virtual const char* what() const
	{
		return _msg.c_str();
	}

protected:

	std::string _msg;
};