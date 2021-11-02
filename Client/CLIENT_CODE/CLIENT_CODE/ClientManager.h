#pragma once


/* Includes */
#include "EncryptionManager.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <map>
#include <functional>

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "cryptopp_wrapper/Base64Wrapper.h"
#include "cryptopp_wrapper/AESWrapper.h"

#include "Utils.h"
#include "Logger.h"
#include "ClientNetworkManager.h"
#include "ClientMenu.h"
#include "Errors.h"
#include "globals.h"
#include "OtherClient.h"


/* Constants and Macros  */

#define		CLIENT_INFO_FILE_NAME		("me.info")
#define		SERVER_INFO_FILE_NAME		("server.info")

#define		CLIENT_ID_PACKED_STR_LEN	(32)
#define		CLIENT_KEY_BASE64_SIZE		(300)





using namespace std;
using namespace boost::uuids;



/*
Client Manager is the Main module that runs the client.
It uses auxiliary modules such as:
	- CLientNetworkManager for communication with server.
	- ClientMenu to run CLI menu and execute handler callback functions. 
	- EnctyptionManager to handler Asymetric enctyption

All exceptions are handled within the module.
*/
class ClientManager : public EncryptionManager
{

public:
	ClientManager();

	
	//************************************************************************************
	// Brief:       Sets up client by using clients configuration file
	// Returns:     void
	// Remarks:      
	//************************************************************************************

	void setup_client();

	//************************************************************************************
	// Brief:       Connecting to server. then runs client CLI menu. asking user for input and performing actions
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void run_client();

	virtual ~ClientManager();

private:


	void _run_console_menu();


	//************************************************************************************
	// Brief:       Sending register request to server. asking for user name
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _request_register();

	//************************************************************************************
	// Brief:       sending request to get clients list
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _get_clients_list();


	//************************************************************************************
	// Brief:       requesting for users public key.
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _get_client_public_key();


	//************************************************************************************
	// Brief:       Asking user for message string and destined user . 
	//				this message is encrypted using symmetric key and sent to the server.
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _send_message_with_content();


	//************************************************************************************
	// Brief:       sending request to server to get user's symmetric key
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _send_request_for_symmetric_key();


	//************************************************************************************
	// Brief:       Sending symmetric key to other user through .the server. this key is encrypted by 
	//				destined user's public key
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _send_symmetric_key();


	//************************************************************************************
	// Brief:       Requesting server to get pending messages. each message is 
	//				decrypted using each  other client's symmetric key
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void _get_pending_messages();


	/* Auxiliary internal functions */

	OtherClient* _find_client_by_name(const char * client_name);

	OtherClient* _find_client_by_id(const uint8_t client_id[CLIENT_ID_SIZE_BYTES]);

private:


	/* Client info */
	bool _is_registered;
	std::string _client_name;
	boost::uuids::uuid _client_id;

	/* Networking info */
	std::string _server_ip;
	std::string _server_port;

	/* Other clients */
	std::vector<OtherClient*> _other_clients;

	/* auxiliary modules */
	ClientNetworkManager _network_manager;
	ClientMenu _menu;
};