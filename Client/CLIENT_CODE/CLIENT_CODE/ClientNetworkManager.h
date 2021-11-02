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




/*  ClientNetworkManager is the ONLY interface to communication with the server.
	there is a function for each request that can be asked from the server.
	the module sends that request by following agreed protocol. then gets the response, 
	parses it and returns relevant output to the user.


	TO Follow protocol. this module uses network PACKED STRUCTS defined files:
	- ServerRequest.h 
	- ServerResponses.h

*/
class ClientNetworkManager : public ClientIOContext
{


public:

	ClientNetworkManager(uuid& client_id) : 
		_client_socket(_io_context),
		_client_id(client_id)
	{
	}


	//************************************************************************************
	// Brief:       Initial connect to server at given ip,port
	// Parameter:   const std::string & ip
	// Parameter:   const std::string & port
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void connect_to_server(const std::string& ip, const std::string& port);



	//************************************************************************************
	// Brief:       Register request 
	// Parameter:   const std::string & name
	// Parameter:   const string & public_key
	// Parameter:   boost::uuids::uuid * received_client_id
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void send_request_register(const std::string& name,
							   const string& public_key,
							   boost::uuids::uuid* received_client_id);



	//************************************************************************************
	// Brief:       Get clients list. insert the information about clients received to given list 
	// Parameter:   std::vector<OtherClient * > & other_clients
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void send_request_get_clients_list(std::vector<OtherClient*>& other_clients);



	//************************************************************************************
	// Brief:       Requests client public key. insert that key to given client
	// Parameter:   uint8_t client_id_requested[CLIENT_ID_SIZE_BYTES]
	// Parameter:   OtherClient * other_client
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void send_request_get_client_public_key(uint8_t client_id_requested[CLIENT_ID_SIZE_BYTES],
											OtherClient* other_client);


	//************************************************************************************
	// Brief:       Send message with content to other client through server
	// Parameter:   uint8_t client_id[CLIENT_ID_SIZE_BYTES]
	// Parameter:   MessageType type
	// Parameter:   const string & message_content
	// Returns:     uint32_t
	// Remarks:      
	//************************************************************************************
	uint32_t send_message_to_client(uint8_t client_id[CLIENT_ID_SIZE_BYTES],
									MessageType type, 
									const string& message_content = "");
	


	//************************************************************************************
	// Brief:       Get waiting messages from server, parse them and insert them to list given
	// Parameter:   vector<Message * > & messages_list
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void get_pending_messages(vector<Message*>& messages_list);

private:

	//************************************************************************************
	// Brief:       General function to parsed request and response. Request given as input is sent to the server. 
	//				response is then received, parsed and returned as out param
	// Parameter:   ServerRequest * request
	// Parameter:   ServerResponse * response
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _process_request_and_response(ServerRequest* request, ServerResponse* response);


	uuid& _client_id;
	tcp::socket _client_socket;

};