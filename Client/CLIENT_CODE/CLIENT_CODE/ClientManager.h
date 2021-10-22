#pragma once


/* Includes */

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

#include "Logger.h"
#include "ClientNetworkManager.h"
#include "ClientMenu.h"

/* Constants and Macros  */

#define		CLIENT_INFO_FILE_NAME	("me.info")
#define		SERVER_INFO_FILE_NAME	("server.info")





// TODO - MOVE TO CPP
using namespace std;


class ClientManager
{

public:
	ClientManager()
	{
		string line;
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
			_server_port = line.substr(separetor_index+1);
		}
	}

	void run_client()
	{
		cout << "Connection to server" << endl;
		_network_manager.connect_to_server(_server_ip, _server_port);

		_run_console_menu();

		cout << "DONE!";
	}


	

private:


	void _run_console_menu()
	{
		std::map <MenuAction, std::function<void()>> action_handler_map = 
		{
			{ACTION_REGISTER, std::bind(&ClientManager::_request_register, this)},
			{ACTION_GET_CLIENTS_LIST, std::bind(&ClientManager::_get_clients_list, this)}
		};

		_menu.run_menu(action_handler_map);
	}

	void _request_register()
	{
		uint8_t public_key[PUBLIC_KEY_SIZE] = { 1, 2, 3, 4, 9 };
		boost::uuids::uuid client_uuid;

		cout << "Registering client" << endl;
		_network_manager.send_request_register("Abba7", public_key, &client_uuid);

		const string uuid_str = boost::lexical_cast<std::string>(client_uuid);
		cout << "Client registered . UUID =" << uuid_str << endl;
	}

	void _get_clients_list()
	{
		cout << "Clients list !!!!" << endl;
	}



	std::string _server_ip;
	std::string _server_port;
	ClientNetworkManager _network_manager;
	ClientMenu _menu;
};