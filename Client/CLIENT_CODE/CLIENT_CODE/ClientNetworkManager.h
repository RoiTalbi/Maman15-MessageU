#pragma once

/* Includes */

#include <string>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Logger.h"
#include "ServerRequests.h"
#include "ServerResponses.h"
#include "server_codes.h"
#include "Errors.h"
#include "OtherClient.h"


/* Constants and Macros */




// TODO - MOVE TO CPP FILE!!

using namespace std;
using boost::asio::ip::tcp;
using boost::uuids::uuid;



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

	ClientNetworkManager(uuid& client_id) : 
		_client_socket(_io_context),
		_client_id(client_id)
	{
	}

	void connect_to_server(const std::string& ip, const std::string& port)
	{
		try
		{
			tcp::resolver resolver(_io_context);
			boost::asio::connect(_client_socket, resolver.resolve(ip, port));
		}
		catch (const boost::exception& ex)
		{
			// TODO - Handle better 
			throw NetworkError("Network Error! " + diagnostic_information(ex));
		}
	}




	void send_request_register(const std::string& name,
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

	void send_request_get_clients_list(std::vector<OtherClient*>& other_clients)
	{
		/* Assemble request for client register . send it and get response */
		ServerResponse response;
		ServerRequest request(REQUEST_CODE_GET_CLIENTS_LIST, 0, _client_id.data);

		_process_request_and_response(&request, &response);

		if (response.code == SERVER_ERROR_CODE)
		{
			throw ServerGeneralError("Server General Error");
		}

		/* Parse response payload -> Other clients list */
		size_t other_clients_count = (response.payload_size / sizeof(ResponseClientInList));
		OtherClient* new_client = NULL;
		ResponseClientInList* client_in_list = NULL;
		bool is_client_is_list = false;

		for (size_t i = 0; i < other_clients_count; i++)
		{
			/* Point to the next client in list within the response payload */
			client_in_list = (ResponseClientInList*)&response.payload[i * sizeof(ResponseClientInList)];
			new_client = new OtherClient(client_in_list->name, client_in_list->client_id);

			/* Find if client already in familiar clients list */
			for (auto client : other_clients)
			{
				if (strncmp(client->name, new_client->name, CLIENT_NAME_SIZE - 1) == 0)
				{
					is_client_is_list = true;
					break;
				}
			}

			/* Insert to familiar clients list only if not there yet */
			if (!is_client_is_list)
			{
				other_clients.push_back(new_client);
			}
			else
			{
				delete new_client;
			}

			is_client_is_list = false;
		}
	}

	void send_request_get_client_public_key(uint8_t client_id_requested[CLIENT_ID_SIZE_BYTES],
											uint8_t out_public_key[PUBLIC_KEY_SIZE])
	{
		/* Assemble request for client public key . send it and get response */
		ServerResponse response;
		ServerRequest request(REQUEST_CODE_GET_CLIENT_PUBLIC_KEY,
							  CLIENT_ID_SIZE_BYTES,
							  _client_id.data,
							  SERVER_VERSION,
							  client_id_requested);
		

		_process_request_and_response(&request, &response);
		if (response.code == SERVER_ERROR_CODE)
		{
			throw ServerGeneralError("Client ID does not exist");
		}

		/* Parse response payload and return received client public key */
		ResponseClientPublicKey* response_payload = (ResponseClientPublicKey *)response.payload;
		memcpy(out_public_key, response_payload->public_key, PUBLIC_KEY_SIZE);
	}




	uint32_t send_message_to_client(uint8_t client_id[CLIENT_ID_SIZE_BYTES],
									MessageType type, 
									const string& message_content)
	{
		uint32_t result_message_id = 0;

		/* Assemble request for client register . send it and get response */
		ServerResponse response;
		ServerRequest request(REQUEST_CODE_SEND_MESSAGE, 
							  sizeof(RequestSendMessage) + message_content.size(),
							  _client_id.data);

		/* Define message type and size . format payload with meta-data and message content */
		RequestSendMessage message_metadata(message_content.size(), client_id, type);

		request.payload = new uint8_t[sizeof(RequestSendMessage) + message_content.size()];
		memcpy(request.payload, &message_metadata, sizeof(RequestSendMessage));

		if (message_content.size() > 0)
		{
			/* If there is content insert it right after message metadata */
			memcpy(&request.payload[sizeof(RequestSendMessage)], message_content.c_str(), message_content.size());
		}

		uint8_t debug[2000];
		memcpy(debug, request.payload, sizeof(RequestSendMessage) + message_content.size());

		_process_request_and_response(&request, &response);
		if (response.code == SERVER_ERROR_CODE)
		{
			throw ServerGeneralError("Client ID does not exist");
		}

		/* Parse response payload and return received message id */
		ResponseMessageSent* response_payload = (ResponseMessageSent*)response.payload;
		result_message_id = response_payload->message_id;

		return result_message_id;
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
			throw NetworkError("Network Error has occurred!");
		}
		catch (const std::bad_alloc& ex)
		{
			if (request_buffer)
				delete request_buffer;

			// TODO - REMOVE
			std::cerr << "Exception: " << ex.what() << "\n";
			throw NetworkError("couldn't allocate enough memory to process network request");
		}

	}


	uuid& _client_id;
	tcp::socket _client_socket;

};