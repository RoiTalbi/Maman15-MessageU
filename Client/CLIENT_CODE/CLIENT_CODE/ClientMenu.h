#pragma once

/* Includes*/
#include <map>
#include <iostream>
#include <string>
#include <functional>
#include <exception>

#include "Logger.h"
#include "Errors.h"






/* Enums and Constants */

#define MAIN_MENU_MESSAGE		   "\n\nMessageU client at your service\n\
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


enum MenuAction
{
	ACTION_EXIT_MENU = 0,
	ACTION_REGISTER = 10,
	ACTION_GET_CLIENTS_LIST = 20,
	ACTION_GET_PUBLIC_KEY = 30,
	ACTION_GET_PENDING_MESSAGES = 40,
	ACTION_SEND_TEXT_MESSAGE = 50,
	ACTION_REQUEST_SYMMETRIC_KEY = 51,
	ACTION_SEND_SYMMETRIC_KEY = 52
};


using namespace std;




/*  ClienMenu module runs the CLI menu in while loop until exit action is entered.
	in each iteration the user is asked for an input through command line. each input 
	number (action number) is transferred than to external handler callback function. that is invoked 
	when action number is entered.
*/
class ClientMenu
{

public:
	ClientMenu()
	{

	}


	//************************************************************************************
	// Brief:       return the user input line entered after printing instruction
	// Parameter:   const string & instruction_message
	// Returns:     std::string
	// Remarks:      
	//************************************************************************************
	string get_user_input(const string& instruction_message);

	//************************************************************************************
	// Brief:       Runs CLI menu. ask user for action and execute matching handle callback
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void run_menu(std::map <MenuAction, std::function<void()>>& action_handlers_map);
};
