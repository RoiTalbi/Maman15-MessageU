#include "EncryptionManager.h"



void EncryptionManager::set_keys(std::string private_key)
{
	_rsa_private = new RSAPrivateWrapper(private_key);
	_private_key = _rsa_private->getPrivateKey();
	_public_key = _rsa_private->getPublicKey();
}

/***********************************************************************************************************/


void EncryptionManager::generate_keys()
{
	_rsa_private = new RSAPrivateWrapper();
	_public_key = _rsa_private->getPublicKey();
	_private_key = _rsa_private->getPrivateKey();
}

/***********************************************************************************************************/



std::string EncryptionManager::encrypt(uint8_t other_public_key[PUBLIC_KEY_SIZE], const char* data, size_t size)
{
	RSAPublicWrapper rsa_other((char*)other_public_key, PUBLIC_KEY_SIZE);
	return rsa_other.encrypt(data, size);
}

/***********************************************************************************************************/


std::string EncryptionManager::decrypt(const char* data, size_t size)
{
	return _rsa_private->decrypt(data, size);
}
