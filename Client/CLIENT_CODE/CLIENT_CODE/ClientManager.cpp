#include "ClientManager.h"



ClientManager::ClientManager() :
	_network_manager(_client_id),
	_is_registered(false)
{
	try
	{
		setup_client();
	}
	catch (const std::exception& ex)
	{
		Logger::LOG_CRITICAL_ERROR(ex.what());
		exit(CLIENT_GENERAL_ERROR_EXIT_CODE);
	}
}

/***********************************************************************************************************/


void ClientManager::setup_client()
{
	string line;

	/* Get server info from server info file */
	ifstream server_info_file(SERVER_INFO_FILE_NAME);
	if (!server_info_file.is_open())
	{
		Logger::LOG_CRITICAL_ERROR("Cannot find server info file");
		exit(CLIENT_GENERAL_ERROR_EXIT_CODE);
	}

	getline(server_info_file, line);
	size_t separetor_index = line.find(':');

	if (separetor_index == string::npos)
	{
		Logger::LOG_CRITICAL_ERROR("Invalid server info file");
		exit(CLIENT_GENERAL_ERROR_EXIT_CODE);
	}
	else
	{
		_server_ip = line.substr(0, separetor_index);
		_server_port = line.substr(separetor_index + 1);
	}

	/* Check if info file already exists (client is registered) . Parse that file if exists */
	ifstream client_info_file(CLIENT_INFO_FILE_NAME);
	string uuid_str = "";
	string tmp = "";
	string private_key_b64 = "";

	if (client_info_file.is_open())
	{
		_is_registered = true;
		getline(client_info_file, _client_name);
		getline(client_info_file, uuid_str);

		// parse the encoded private key
		while (getline(client_info_file, tmp)) {
			private_key_b64 += tmp;
		}

		string private_key_str = Base64Wrapper::decode(private_key_b64);
		if (uuid_str.size() != CLIENT_ID_PACKED_STR_LEN)
		{
			throw ClientError("Invalid me.info file");
		}

		/* Setup client by these parameters */
		_client_id = Utils::str_to_uuid(uuid_str);
		set_keys(private_key_str);
	}
}

/***********************************************************************************************************/

void ClientManager::run_client()
{
	Logger::LOG_INFO("Connection to server");
	try
	{
		_network_manager.connect_to_server(_server_ip, _server_port);
	}
	catch (const ClientError& ex)
	{
		Logger::LOG_CRITICAL_ERROR("Could not connect to server", ex.what());
		exit(CLIENT_NETWORK_ERROR_EXIT_CODE);
	}

	_run_console_menu();
}

/***********************************************************************************************************/


ClientManager::~ClientManager()
{
	/* Release all dynamic allocated memory */
	for (auto client : _other_clients)
	{
		delete client;
	}
}

/***********************************************************************************************************/



void ClientManager::_run_console_menu()
{
	std::map <MenuAction, std::function<void()>> action_handler_map =
	{
		{ACTION_REGISTER, std::bind(&ClientManager::_request_register, this)},
		{ACTION_GET_CLIENTS_LIST, std::bind(&ClientManager::_get_clients_list, this)},
		{ACTION_GET_PUBLIC_KEY, std::bind(&ClientManager::_get_client_public_key, this)},
		{ACTION_SEND_TEXT_MESSAGE, std::bind(&ClientManager::_send_message_with_content, this)},
		{ACTION_GET_PENDING_MESSAGES, std::bind(&ClientManager::_get_pending_messages, this)},
		{ACTION_SEND_SYMMETRIC_KEY, std::bind(&ClientManager::_send_symmetric_key, this)},
		{ACTION_REQUEST_SYMMETRIC_KEY, std::bind(&ClientManager::_send_request_for_symmetric_key, this)}
	};

	_menu.run_menu(action_handler_map);
}

/***********************************************************************************************************/



void ClientManager::_request_register()
{
	if (_is_registered)
	{
		throw ClientError("Client already registered");
	}

	string user_name = _menu.get_user_input("Enter User Name");
	Logger::LOG_INFO("Registering client");

	generate_keys();
	_network_manager.send_request_register(user_name, _public_key, &_client_id);

	string uuid_str = Utils::uuid_to_packed_str(_client_id);
	Logger::LOG_INFO("Client registered . UUID=", uuid_str);

	/* Save registration info and private key to client info file */
	ofstream client_info_file(CLIENT_INFO_FILE_NAME);
	client_info_file << user_name << endl;
	client_info_file << uuid_str << endl;
	client_info_file << Base64Wrapper::encode(_private_key) << endl;

	_is_registered = true;
}

/***********************************************************************************************************/


void ClientManager::_get_clients_list()
{
	_network_manager.send_request_get_clients_list(_other_clients);

	cout << "------- Clients list -------" << endl;
	for (auto client : _other_clients)
	{
		cout << "Client Name: " << client->get_name() << "  \t\tID: " <<
			Utils::raw_bytes_to_uuid_str(client->get_client_id()) << endl;
	}
	cout << "----------------------------" << endl;
}


/***********************************************************************************************************/

void ClientManager::_get_client_public_key()
{
	string requested_client_name = _menu.get_user_input("Enter Client Name");

	/* Search that client with requested id within the familiar clients list. get it's uid */
	OtherClient* requested_client = _find_client_by_name(requested_client_name.c_str());

	if (!requested_client)
	{
		throw ClientError("Client information hasn't been received from server");
	}

	_network_manager.send_request_get_client_public_key(requested_client->get_client_id(), requested_client);
	Logger::LOG_INFO("Public key received successfully");
}

/***********************************************************************************************************/



void ClientManager::_send_message_with_content()
{
	uint32_t message_id = 0;
	string requested_client_name = _menu.get_user_input("Enter Client Name");
	OtherClient* requested_client = _find_client_by_name(requested_client_name.c_str());

	if (!requested_client)
	{
		throw ClientError("Client information hasn't been received from server");
	}
	else if (!requested_client->is_symmetric_key_present())
	{
		throw ClientError("Symmetric key not received or generated");
	}

	/* Get message from user. encrypt it and send it to server */
	string content = _menu.get_user_input("Enter Message Content");

	AESWrapper aes(requested_client->get_symmetric_key(), SYMMETRIC_KEY_SIZE);
	string cipher_content = aes.encrypt(content.c_str(), content.length());
	message_id = _network_manager.send_message_to_client(requested_client->get_client_id(), MessageType::REGULAR_MESSAGE, cipher_content);

	Logger::LOG_INFO("Message sent successfully", "Message ID= " + message_id);
}

/***********************************************************************************************************/



void ClientManager::_send_request_for_symmetric_key()
{
	uint32_t message_id = 0;
	string requested_client_name = _menu.get_user_input("Enter Client Name");
	OtherClient* requested_client = _find_client_by_name(requested_client_name.c_str());

	if (!requested_client)
	{
		throw ClientError("Client information hasn't been received from server");
	}

	_network_manager.send_message_to_client(requested_client->get_client_id(), MessageType::GET_SYMMETRIC_KEY);
}

/***********************************************************************************************************/



void ClientManager::_send_symmetric_key()
{
	uint32_t message_id = 0;
	AESWrapper aes;

	string requested_client_name = _menu.get_user_input("Enter Client Name");
	OtherClient* requested_client = _find_client_by_name(requested_client_name.c_str());
	if (!requested_client)
	{
		throw ClientError("Client information hasn't been received from server");
	}
	else if (!requested_client->is_public_key_received())
	{
		throw ClientError("Client's public key should be received first");
	}

	/* Generate symmetric key for that client if not present and save that key in memory*/
	if (!requested_client->is_symmetric_key_present())
	{
		requested_client->set_symmetric_key(
			aes.GenerateKey(requested_client->get_symmetric_key(), SYMMETRIC_KEY_SIZE));

	}

	/* Encrypt that symmetric key with public key , and send it to the user */
	string encrypted_symmetric_key = encrypt(requested_client->get_public_key(),
		(char*)requested_client->get_symmetric_key(),
		SYMMETRIC_KEY_SIZE);

	_network_manager.send_message_to_client(requested_client->get_client_id(), MessageType::SYMMETRIC_KEY_SENT, encrypted_symmetric_key);
}

/***********************************************************************************************************/



void ClientManager::_get_pending_messages()
{
	vector<Message*> messages_received;
	string symmetric_key = "";

	/* Retrieve all messages from server and present them to the client */
	_network_manager.get_pending_messages(messages_received);

	cout << endl << "--------------------";
	for (auto message : messages_received)
	{
		OtherClient* sender = _find_client_by_id(message->sender_client_id);
		if (!sender)
		{
			throw ClientError("Clients information hasn't been received from server");
		}

		cout << endl << "From: " << sender->get_name() << endl;

		switch (message->type)
		{
		case MessageType::GET_SYMMETRIC_KEY:
			cout << "Request for symmetric key" << endl;
			break;

		case MessageType::SYMMETRIC_KEY_SENT:

			cout << "Symmetric key received" << endl;

			/* Decrypt symmetric key and save that key of the specific client */
			symmetric_key = decrypt((char*)message->content, message->content_size);
			sender->set_symmetric_key((uint8_t*)symmetric_key.c_str());
			break;

		case MessageType::REGULAR_MESSAGE:
			string text_message = "";

			if (!sender->is_symmetric_key_present())
			{
				cout << "Can't decrypt message" << endl;
				break;
			}
			else
			{
				AESWrapper aes(sender->get_symmetric_key(), SYMMETRIC_KEY_SIZE);
				text_message = aes.decrypt((char*)message->content, message->content_size);
			}

			cout << "Content:" << endl << text_message << endl;
			break;
		}

		cout << "--------<EOM>--------" << endl;
	}

	/* Delete all printed messages */
	for (auto message : messages_received)
	{
		delete message;
	}
}


/***********************************************************************************************************/


OtherClient* ClientManager::_find_client_by_name(const char* client_name)
{
	/* Find if client already in familiar clients list */
	for (auto client : _other_clients)
	{
		if (strncmp(client->get_name(), client_name, CLIENT_NAME_SIZE - 1) == 0)
		{
			return client;
		}
	}

	return NULL;
}

/***********************************************************************************************************/


OtherClient* ClientManager::_find_client_by_id(const uint8_t client_id[CLIENT_ID_SIZE_BYTES])
{
	/* Find if client already in familiar clients list */
	for (auto client : _other_clients)
	{
		if (memcmp(client->get_client_id(), client_id, CLIENT_ID_SIZE_BYTES) == 0)
		{
			return client;
		}
	}

	return NULL;
}
