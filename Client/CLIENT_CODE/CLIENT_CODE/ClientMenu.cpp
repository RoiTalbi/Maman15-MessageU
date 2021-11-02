#include "ClientMenu.h"



std::string ClientMenu::get_user_input(const string& instruction_message)
{
	string input = "";
	string line = "";

	cout << instruction_message << endl;

	// Get a full line if input from the user 
	do
	{
		getline(cin, line);
		input += line;

	} while (line == "");

	return input;
}


/***********************************************************************************************************/


void ClientMenu::run_menu(std::map <MenuAction, std::function<void()>>& action_handlers_map)
{
	string user_choice = "";
	MenuAction menu_choice = ACTION_EXIT_MENU;

	while (true)
	{
		// Scan for user's choice
		cout << MAIN_MENU_MESSAGE << endl << endl;
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

		/* Execute matching handler to user's request according to action handlers map */ 
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
		catch (const std::exception& ex)
		{
			Logger::LOG_ERROR("GENERAL ERROR:  ", ex.what());
		}


	}
}
