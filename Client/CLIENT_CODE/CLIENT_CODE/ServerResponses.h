#pragma once


/* Includes */
#include <stdint.h>
#include "globals.h"





/* Constants and Macros */
#define RESPONSE_PACKET_HEADERS_SIZE	(7)

#define RESPONSE_MESSAGE_HEADERS_SIZE	(25)






/* Server Generic Response Protocol Headers */

#pragma pack(1) 
struct ServerResponse
{

public:
	uint8_t version;
	uint16_t code;
	uint32_t payload_size;

	uint8_t* payload;


	ServerResponse() : version(SERVER_VERSION), code(0), payload(NULL)
	{
	}

	/* Destructor cannot be virtual! since using pragma pack.
	   Need to avoid saving _vtable
	*/
	~ServerResponse()
	{
		if (payload)
		{
			delete payload;
		}
	}
};




/* ----------- All other server responses structures  ----------- */

#pragma pack(1)
struct ResponseClientInList
{
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	char name[CLIENT_NAME_SIZE];
};


#pragma pack(1) 
struct ResponseClientPublicKey
{
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	uint8_t public_key[PUBLIC_KEY_SIZE];
};



#pragma pack(1) 
struct ResponseMessageSent
{
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	uint32_t message_id;
};
