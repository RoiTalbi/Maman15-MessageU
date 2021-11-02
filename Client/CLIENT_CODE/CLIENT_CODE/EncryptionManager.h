#pragma once

#include "cryptopp_wrapper/Base64Wrapper.h"
#include "cryptopp_wrapper/RSAWrapper.h"
#include "cryptopp_wrapper/AESWrapper.h"

#include <string>

#include "globals.h"








/*  EncryptionManager module is responsible for Asymmetric encryption. 
	ClientManager can extend this module and use it to encrypt/decrypt messages and store keys.
*/
class EncryptionManager
{
	

public:
	EncryptionManager() : _rsa_private(NULL) 
	{}


	//************************************************************************************
	// Brief:       Set private key and public key - if they exist already 
	// Parameter:   std::string private_key
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void set_keys(std::string private_key);


	//************************************************************************************
	// Brief:       Generate new Keys - public & private
	// Returns:     void
	// Remarks:      
	//************************************************************************************
	void generate_keys();


	//************************************************************************************
	// Brief:       Encrypt buffer of data. return ciphertext
	// Parameter:   uint8_t other_public_key[PUBLIC_KEY_SIZE]
	// Parameter:   const char * data
	// Parameter:   size_t size
	// Returns:     std::string
	// Remarks:      
	//************************************************************************************
	std::string encrypt(uint8_t other_public_key[PUBLIC_KEY_SIZE], const char* data, size_t size);

	//************************************************************************************
// Brief:       Decrypt buffer of data. return plaintext
// Returns:     
// Remarks:      
//************************************************************************************
	std::string decrypt(const char* data, size_t size);




	~EncryptionManager()
	{
		delete _rsa_private;
	}
	

protected:

	RSAPrivateWrapper* _rsa_private;
	std::string  _public_key;
	std::string  _private_key;
};