


#include <string>
#include <iostream>

#include "cryptopp_wrapper/Base64Wrapper.h"
#include "cryptopp_wrapper/RSAWrapper.h"
#include "cryptopp_wrapper/AESWrapper.h"


#include "ClientNetworkManager.h"
#include "ClientManager.h"




using namespace std;



int main()
{
	ClientManager client;
	client.run_client();
}