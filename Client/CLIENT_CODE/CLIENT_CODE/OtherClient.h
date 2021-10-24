#pragma once

#include "globals.h"

struct OtherClient

{
	char name[CLIENT_NAME_SIZE];
	uint8_t client_id[CLIENT_ID_SIZE_BYTES];
	uint8_t public_key[PUBLIC_KEY_SIZE];

	OtherClient(char name[CLIENT_NAME_SIZE], uint8_t client_id[CLIENT_ID_SIZE_BYTES]) : public_key{ 0 }
	{
		memcpy(this->name, name, CLIENT_NAME_SIZE);
		memcpy(this->client_id, client_id, CLIENT_ID_SIZE_BYTES);
	}
};
