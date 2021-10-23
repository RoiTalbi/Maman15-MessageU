#pragma once

/* Includes */
#include <exception>
#include <string>



/* Error Codes */

#define CLIENT_GENERAL_ERROR_EXIT_CODE		(1)
#define CLIENT_NETWORK_ERROR_EXIT_CODE		(2)



/* Module Exceptions */



class ClientError : public std::exception
{
public:
	ClientError() {};

	ClientError(const char* message) : _msg(message) {}

	ClientError(const std::string& message) : _msg(message) {}

	virtual ~ClientError() noexcept {}


	virtual const char* what() const
	{
		return _msg.c_str();
	}

	std::string _msg;
};



class ServerGeneralError : public ClientError
{
public:

	ServerGeneralError(const char* message) : ClientError(message) {}

	ServerGeneralError(const std::string& message) : ClientError(message) {}

	virtual ~ServerGeneralError() noexcept {}

};



class NetworkError : public ClientError
{
public:

	NetworkError(const char* message) : ClientError(message) {}

	NetworkError(const std::string& message) : ClientError(message) {}

	virtual ~NetworkError() noexcept {}

};