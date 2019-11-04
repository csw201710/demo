#pragma once
#include "common.h"

#include <string>
using namespace std;

class RSAHandler
{
	int m_padding;
	RSA * m_pubkey;
	RSA * m_prikey;
public:
	RSAHandler();
	virtual ~RSAHandler();
	int generateRSA();

	int setPubKey(RSA* rsa);
	int setPriKey(RSA* rsa);

	int public_encrypt(std::string data , std::string &encrypted);
	int private_decrypt(std::string data, std::string &decrypted);
	int private_encrypt(std::string data, std::string &encrypted);
	int public_decrypt(std::string data, std::string &decrypted);

};

