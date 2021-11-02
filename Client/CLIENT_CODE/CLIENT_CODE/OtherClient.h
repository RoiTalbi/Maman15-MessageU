#pragma once

#include "globals.h"

class OtherClient

{

public:

	OtherClient(char name[CLIENT_NAME_SIZE], uint8_t client_id[CLIENT_ID_SIZE_BYTES]) : 
		_public_key{ 0 },
		_symmetric_key{ 0 },
		_symmetric_key_present{false},
		_public_key_received{false}
	{
		memcpy(this->_name, name, CLIENT_NAME_SIZE);
		memcpy(this->_client_id, client_id, CLIENT_ID_SIZE_BYTES);
	}

	
	/* Setters and getters */
	char* get_name()
	{
		return _name;
	}

	uint8_t* get_client_id()
	{
		return _client_id;
	}

	uint8_t* get_public_key()
	{
		return _public_key;
	}

	uint8_t* get_symmetric_key()
	{
		return _symmetric_key;
	}

	void set_symmetric_key(uint8_t symmetric_key[SYMMETRIC_KEY_SIZE])
	{
		_symmetric_key_present = true;
		memcpy(_symmetric_key, symmetric_key, SYMMETRIC_KEY_SIZE);
	}

	void set_public_key(uint8_t public_key[PUBLIC_KEY_SIZE])
	{
		_public_key_received = true;
		memcpy(_public_key, public_key, PUBLIC_KEY_SIZE);
	}

	bool is_public_key_received()
	{
		return _public_key_received;
	}

	bool is_symmetric_key_present() 
	{
		return _symmetric_key_present;
	}



private:
	char _name[CLIENT_NAME_SIZE];
	uint8_t _client_id[CLIENT_ID_SIZE_BYTES];
	uint8_t _public_key[PUBLIC_KEY_SIZE];
	uint8_t _symmetric_key[SYMMETRIC_KEY_SIZE];

	bool _symmetric_key_present;
	bool _public_key_received;



};
