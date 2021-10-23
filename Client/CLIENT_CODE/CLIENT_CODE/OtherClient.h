#pragma once


#include "globals.h"



#pragma pack(1)
struct OtherClient
{
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	char name[CLIENT_NAME_SIZE];
};