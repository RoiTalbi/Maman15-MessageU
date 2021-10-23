#pragma once

/* Includes*/

#include <map>
#include <iostream>
#include <string>
#include <functional>



#define MAIN_MENU_MESSAGE		   "MessageU client at your service\n\
10) Register\n\
20) Request for clients list\n\
30) Request for public key\n\
40) Request for waiting messages\n\
50) Send a text message\n\
51) Send a request for symmetric key\n\
52) Send your symmetric key\n\
0) Exit client\n?"



#define MESSAGE_INVALID_CHOICE		("Invalid choice. Please enter choice again\n")
#define MESSAGE_SERVER_ERROR		("Server responded with an error: ")



/* Enums and Constants */
enum MenuAction
{
	ACTION_EXIT_MENU = 0,
	ACTION_REGISTER = 10,
	ACTION_GET_CLIENTS_LIST = 20,
	ACTION_GET_PUBLIC_KEY = 30,
	ACTION_GET_WAITING_MESSAGES = 40,
	ACTION_SEND_TEXT_MESSAGE = 50,
	ACTION_REQUEST_SYMMETRIC_KEY = 51,
	ACTION_SEND_SYMMETRIC_KEY = 52
};


using namespace std;


/* Classes */

class ClientMenu
{

public:
	ClientMenu()
	{

	}

	string get_user_input(const string& instruction_message)
	{
		string input = "";

		cout << instruction_message << endl;
		cin >> input;

		return input;
	}


	void run_menu(std::map <MenuAction, std::function<void()>>& action_handlers_map)
	{
		string user_choice= "";
		MenuAction menu_choice = ACTION_EXIT_MENU;

		while (true)
		{
			// Scan for user's choice
			cout << MAIN_MENU_MESSAGE << endl;
			cin >> user_choice;

			try 
			{
				menu_choice = (MenuAction)::stoi(user_choice);
			}
			catch (const std::exception& ex)
			{
				cout << MESSAGE_INVALID_CHOICE;
				continue;
			}

			// make sure user's choice is valid action 
			if (menu_choice == ACTION_EXIT_MENU)
			{
				break;
			}
			else if (action_handlers_map.find(menu_choice) == action_handlers_map.end())
			{
				cout << MESSAGE_INVALID_CHOICE;
				continue;
			}

			//Execute matching handler to user's request according to action handlers map
			try
			{
				action_handlers_map[menu_choice]();
			}
			catch (const ServerGeneralError& ex)
			{
				cout << MESSAGE_SERVER_ERROR << ex.what() << endl;
			}
			catch (const ClientError& ex)
			{
				Logger::LOG_ERROR(ex.what());
			}
			
			
		}
	}
};
