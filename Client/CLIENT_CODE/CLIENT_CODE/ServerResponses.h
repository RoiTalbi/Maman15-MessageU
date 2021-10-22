#pragma once


/* Includes */
#include <stdint.h>
#include "globals.h"





/* Constants and Macros */
#define RESPONSE_PACKET_HEADERS_SIZE	(7)








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
