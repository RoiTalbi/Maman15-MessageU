#pragma once

/* Includes */

#include <string>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/exception/all.hpp>

#include "ServerRequests.h"
#include "ServerResponses.h"
#include "server_codes.h"
#include "Errors.h"

/* Constants and Macros */




// TODO - MOVE TO CPP FILE!!

using boost::asio::ip::tcp;




/* 
Auxiliary class makes sure that io_context is initialized 
 before other ClientNetworkManager variables
*/
class ClientIOContext
{
protected:
	boost::asio::io_context _io_context;
};




class ClientNetworkManager : public ClientIOContext
{


public:

	ClientNetworkManager(const std::string& ip, const std::string& port) : 
		_client_socket(_io_context)
	{
		try
		{
			tcp::resolver resolver(_io_context);
			boost::asio::connect(_client_socket, resolver.resolve(ip, port));
		}
		catch (const boost::exception& ex)
		{
			// TODO - Handle better 

			std::cerr << "Exception: " << boost::diagnostic_information(ex) << "\n";
			throw ClientNetworkError("Network Error!!");
		}
	}


	void send_request_register(std::string name,
							   uint8_t public_key[PUBLIC_KEY_SIZE],
							   boost::uuids::uuid* received_client_id)
	{
		uint8_t temp_client_id[CLIENT_ID_SIZE_BYTES] = { 0 };
		ServerResponse response;

		/* Assemble request for client register . send it and get response */
		ServerRequest request(REQUEST_CODE_REGISTER, sizeof(RequestRegister), temp_client_id);
		RequestRegister register_request_payload(name.c_str(), public_key);

		request.payload = (uint8_t*)(&register_request_payload);

		_process_request_and_response(&request, &response);


		/* Return received client_ID from server or throw error if occurred */
		if (response.code == SERVER_ERROR_CODE)
		{
			throw ServerGeneralError("Server Error- Client with that name already exists");
		}
		else
		{
			memcpy(received_client_id->data, response.payload, CLIENT_ID_SIZE_BYTES);
		}
	}



private:


	void _process_request_and_response(ServerRequest* request, ServerResponse* response)
	{
		uint8_t* request_buffer = NULL;

		try
		{
			/* Allocate memory for request and send it through socket */
			request_buffer = new uint8_t[REQUEST_PACKET_HEADERS_SIZE + request->payload_size];

			/* Send request */
			memcpy(request_buffer, request, REQUEST_PACKET_HEADERS_SIZE);
			memcpy(&request_buffer[REQUEST_PACKET_HEADERS_SIZE], request->payload, request->payload_size);

			boost::asio::write(_client_socket, boost::asio::buffer(request_buffer,
																   REQUEST_PACKET_HEADERS_SIZE + request->payload_size));


			/* Read response headers and response payload if has any payload*/
			boost::asio::read(_client_socket, boost::asio::buffer((uint8_t*)response, RESPONSE_PACKET_HEADERS_SIZE));

			if (response->payload_size)
			{
				/* Allocate enough memory to payload size and read that payload from socket */
				response->payload = new uint8_t[response->payload_size];
				boost::asio::read(_client_socket, boost::asio::buffer(response->payload, response->payload_size));
			}
		}

		catch (const boost::exception& ex)
		{
			if (request_buffer)
				delete request_buffer;

			// TODO - REMOVE
			std::cerr << "Exception: " << boost::diagnostic_information(ex) << "\n";
			throw ClientNetworkError("Network Error has occurred!");
		}
		catch (const std::bad_alloc& ex)
		{
			if (request_buffer)
				delete request_buffer;

			// TODO - REMOVE
			std::cerr << "Exception: " << ex.what() << "\n";
			throw ClientNetworkError("couldn't allocate enough memory to process network request");
		}

	}


	tcp::socket _client_socket;

};