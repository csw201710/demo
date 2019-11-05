#pragma once
#include "common.h"

#include <string>
using namespace std;

class RSAHandler
{
private:
	int m_padding;
	RSA * m_pubkey;
	RSA * m_prikey;
	int _encrypt(std::string&  data, std::string & encrypted, int isprivate);
	int _decrypt(std::string& data, std::string & decrypted, int isprivate);
public:
	RSAHandler();
	virtual ~RSAHandler();
	int generateRSA();

	int setPubKey(RSA* rsa);
	int setPriKey(RSA* rsa);

	int public_encrypt(std::string& data , std::string &encrypted);
	int private_decrypt(std::string& data, std::string &decrypted);
	int private_encrypt(std::string& data, std::string &encrypted);
	int public_decrypt(std::string& data, std::string &decrypted);

	int private_sign(std::string& data, std::string &signData);
	
	int public_verify(std::string& data, std::string &signData);
	
	RSA * createRSA(unsigned char * key, int ispubkey);
};

