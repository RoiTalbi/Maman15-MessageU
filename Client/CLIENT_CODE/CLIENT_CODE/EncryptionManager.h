#pragma once

#include "cryptopp_wrapper/Base64Wrapper.h"
#include "cryptopp_wrapper/RSAWrapper.h"
#include "cryptopp_wrapper/AESWrapper.h"

#include <string>

#include "globals.h"


class EncryptionManager
{
	

public:
	EncryptionManager() : _rsa_private(NULL) 
	{

	}


	void set_keys(std::string private_key)
	{
		_rsa_private = new RSAPrivateWrapper(private_key);
		//_rsa_public = new RSAPublicWrapper(_rsa_private->getPublicKey());

		_private_key = _rsa_private->getPrivateKey();
		_public_key = _rsa_private->getPublicKey();
	}

	void generate_keys()
	{
		_rsa_private = new RSAPrivateWrapper();
		//_rsa_public = new RSAPublicWrapper(_rsa_private->getPublicKey());

		_public_key = _rsa_private->getPublicKey();	
		_private_key = _rsa_private->getPrivateKey();
	}


	std::string encrypt(uint8_t other_public_key[PUBLIC_KEY_SIZE], const char* data, size_t size)
	{
		RSAPublicWrapper rsa_other((char*)other_public_key, PUBLIC_KEY_SIZE);
		return rsa_other.encrypt(data, size);
	}

	std::string decrypt(const char* data, size_t size)
	{
		return _rsa_private->decrypt(data, size);
	}

	~EncryptionManager()
	{
		delete _rsa_private;
		//delete _rsa_public;
	}
	

protected:

	//RSAPublicWrapper* _rsa_public;
	RSAPrivateWrapper* _rsa_private;

	std::string  _public_key;
	std::string  _private_key;
};