#include "ClientNetworkManager.h"




void ClientNetworkManager::connect_to_server(const std::string& ip, const std::string& port)
{
	try
	{
		tcp::resolver resolver(_io_context);
		boost::asio::connect(_client_socket, resolver.resolve(ip, port));
	}
	catch (const boost::exception& ex)
	{
		throw NetworkError("Network Error! " + diagnostic_information(ex));
	}
}

/***********************************************************************************************************/



void ClientNetworkManager::send_request_register(const std::string& name,
												 const string& public_key,
											 	 boost::uuids::uuid* received_client_id)
{
	uint8_t temp_client_id[CLIENT_ID_SIZE_BYTES] = { 0 };
	ServerResponse response;

	/* Assemble request for client register . send it and get response */
	ServerRequest request(REQUEST_CODE_REGISTER, sizeof(RequestRegister), temp_client_id);
	RequestRegister register_request_payload(name.c_str(), (uint8_t*)public_key.c_str());
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

/***********************************************************************************************************/


void ClientNetworkManager::send_request_get_clients_list(std::vector<OtherClient*>& other_clients)
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
		client_in_list = (ResponseClientInList*)& response.payload[i * sizeof(ResponseClientInList)];
		new_client = new OtherClient(client_in_list->name, client_in_list->client_id);

		/* Find if client already in familiar clients list */
		for (auto client : other_clients)
		{
			if (strncmp(client->get_name(), new_client->get_name(), CLIENT_NAME_SIZE - 1) == 0)
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

/***********************************************************************************************************/


void ClientNetworkManager::send_request_get_client_public_key(uint8_t client_id_requested[CLIENT_ID_SIZE_BYTES],
														      OtherClient* other_client)
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
	ResponseClientPublicKey* response_payload = (ResponseClientPublicKey*)response.payload;
	other_client->set_public_key(response_payload->public_key);
}


/***********************************************************************************************************/


uint32_t ClientNetworkManager::send_message_to_client(uint8_t client_id[CLIENT_ID_SIZE_BYTES],
													  MessageType type,
													  const string& message_content /*= ""*/)
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


/***********************************************************************************************************/


void ClientNetworkManager::get_pending_messages(vector<Message*>& messages_list)
{
	ServerResponse response;
	ServerRequest request(REQUEST_CODE_GET_PENDING_MESSAGES, 0, _client_id.data);
	size_t response_payload_cursor = 0;

	_process_request_and_response(&request, &response);
	if (response.code == SERVER_ERROR_CODE)
	{
		throw ServerGeneralError("Error getting client's messages");
	}

	/* Parse response payload - add all messages to message list given */
	while (response_payload_cursor < response.payload_size)
	{
		Message* message_in_response = (Message*)& response.payload[response_payload_cursor];

		/* Create new message based on one in response. Deep copy it's content buffer!*/
		Message* message = new Message(message_in_response);
		message->content = new uint8_t[message->content_size];
		memcpy(message->content,
			&response.payload[response_payload_cursor + RESPONSE_MESSAGE_HEADERS_SIZE],
			message_in_response->content_size);


		messages_list.push_back(message);
		response_payload_cursor += (RESPONSE_MESSAGE_HEADERS_SIZE + message_in_response->content_size);
	}
}



/***********************************************************************************************************/



void ClientNetworkManager::_process_request_and_response(ServerRequest* request, ServerResponse* response)
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

		throw NetworkError("Network Error has occurred!");
	}
	catch (const std::bad_alloc& ex)
	{
		if (request_buffer)
			delete request_buffer;

		throw NetworkError("couldn't allocate enough memory to process network request");
	}
}

/***********************************************************************************************************/


