
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>

#include "ClientNetworkManager.h"

using namespace std;

int main()
{
	cout << "Connection to server" << endl;

	ClientNetworkManager network_manager("localhost", "5558");
	uint8_t public_key[PUBLIC_KEY_SIZE] = { 1, 2, 3, 4, 9 };
	boost::uuids::uuid client_uuid;

	cout << "Registering client" << endl;
	network_manager.send_request_register("Abba7", public_key, &client_uuid);


	const string uuid_str = boost::lexical_cast<std::string>(client_uuid);
	cout << "Client registered . UUID =" << uuid_str << endl;

	cout << "DONE!";
}