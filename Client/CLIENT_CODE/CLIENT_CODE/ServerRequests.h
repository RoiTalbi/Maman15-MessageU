#pragma once

/* Includes */
#include <stdint.h>

#include "globals.h"





/* Constants and Macros */
#define REQUEST_PACKET_HEADERS_SIZE		(23)








/* Server Generic Request Protocol Headers */

#pragma pack(1) 
struct ServerRequest
{
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	uint8_t version;
	uint16_t code;
	uint32_t payload_size;

	uint8_t* payload;

public:
	ServerRequest(uint16_t code, 
				  uint32_t payload_size,
				  uint8_t client_id[CLIENT_ID_SIZE_BYTES],
				  uint8_t version = SERVER_VERSION,
				  uint8_t * payload = NULL)
	{
		this->code = code;
		this->payload_size = payload_size;
		this->version = version;
		this->payload = payload;
		memcpy(this->client_id, client_id, CLIENT_ID_SIZE_BYTES);
	}
};




/* ----------- All other server requests structures  ----------- */


#pragma pack(1) 
struct RequestRegister
{
	RequestRegister(const char* name, uint8_t  public_key[PUBLIC_KEY_SIZE])
	{
		strncpy_s(this->name, name, CLIENT_NAME_SIZE - 1);
		memcpy(this->public_key, public_key, PUBLIC_KEY_SIZE);
	}

	char name[CLIENT_NAME_SIZE];
	uint8_t public_key[PUBLIC_KEY_SIZE];
};
