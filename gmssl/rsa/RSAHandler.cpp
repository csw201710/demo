#include "RSAHandler.h"

#include "../crypto/rsa/rsa_locl.h"
RSAHandler::RSAHandler()
{
	m_padding = RSA_PKCS1_PADDING;
}


RSAHandler::~RSAHandler()
{
}

int RSAHandler::setPubKey(RSA* rsa) {
	if (rsa) {
		m_pubkey = rsa;
		return 0;
	}

	return -1;
}

int RSAHandler::setPriKey(RSA* rsa) {
	if (rsa) {
		m_prikey = rsa;
		return 0;
	}
	
	return -1;
}

int RSAHandler::generateRSA() {
	RSA *rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);
	if (rsa == NULL)
	{
		printf("RSA_generate_key error!\n");
		return -1;
	}
	m_prikey = m_pubkey = rsa;
	return 0;
}

int RSAHandler::public_encrypt(std::string  data, std::string & encrypted)
{
	int result = 0;
	if (m_pubkey == 0) {
		printf("m_pubkey can not be null\n");
		return -1;
	}
	RSA * rsa = m_pubkey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int rsasize = RSA_size(rsa);//获取rsa->n 大小

		int exppadding = rsasize;
		int result = -1;
		 char*  indata =(char*) data.data();
		int data_len = data.length();
		//.net Framework中提供的RSA算法规定，每次加密的字节数，不能超过密钥的长度值减去11,
		//而每次加密得到的密文长度，却恰恰是密钥的长度。
		/*
		1.RSA_PKCS1_PADDING 填充模式，最常用的模式

		要求:
		输入：必须 比 RSA 钥模长(modulus) 短至少11个字节, 也就是　RSA_size(rsa) – 11
		如果输入的明文过长，必须切割，　然后填充

		输出：和modulus一样长

		根据这个要求，对于512bit的密钥，　block length = 512/8 – 11 = 53 字节
		*/

		//if (data_len>exppadding - 11)
		exppadding = exppadding - 11;
		int b = 0;
		int s = data_len / (exppadding);
		if (data_len % (exppadding))
			s++;

		char* subdata =(char*) malloc(rsasize);
		char* encryptData = (char*)malloc(rsasize);
		encrypted.clear();
		result = 0;
		for (int i = 0; i < s; i++)//exppadding 每次的需要加密数据长度
		{
			memset(encryptData, 0x00, rsasize);
			memset(subdata, 0x00, rsasize);

			for (int j = 0;j<exppadding;j++)
			{
				if (i*exppadding + j>data_len) //超过原始数据长度跳出
					break;
				subdata[j] = indata[j + i*exppadding];
			}
			// exppadding : 待加密数据长度
			// smldata    : 待加密数据
			// smlencrypted : 存放密文数据
			//  rsa      : 存放密钥
			// 
			
			int ret = RSA_public_encrypt(exppadding, (const unsigned char*)subdata, (unsigned char*)encryptData, rsa, RSA_PKCS1_PADDING);
			if (ret>0)
			{
				encrypted.append(encryptData, rsasize);
				result += ret;
			}
			else {
				printf("%s\n", ERR_error_string(ERR_get_error(), NULL));

			}

			
		}
		if (encryptData) { free(encryptData); }
		if (subdata) { free(subdata); };		
	}

	return result;
}

int RSAHandler::private_decrypt(std::string data, std::string & decrypted)
{
	int result = 0;
	if (m_prikey == 0) {
		printf("m_prikey can not be null\n");
		return -1;
	}
	RSA * rsa = m_prikey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int data_len = data.length();
		char * encdata = (char*)data.data();

		int rsasize = RSA_size(rsa);
		int s = data_len / (rsasize);
		if (data_len % (rsasize))
			s++;
		char* subdata = (char*)malloc(rsasize);
		char* decryptData = (char*)malloc(rsasize);
		decrypted.clear();

		for (int i = 0;i < s;i++)
		{
			for (int j = 0;j<rsasize;j++)
			{
				if (i*rsasize + j>data_len)
					break;
				subdata[j] = encdata[j + i*rsasize];
			}
			unsigned char smlencrypted[1024] = { 0 };
			int ret = RSA_private_decrypt(rsasize, (unsigned char *)subdata, (unsigned char*)decryptData, rsa, RSA_PKCS1_PADDING);
			if (ret>0)
			{
				decrypted.append(decryptData, ret);
				result += ret;
			}
		}
		if (decryptData) { free(decryptData); }
		if (subdata) { free(subdata); };
	}

	return result;
}




int RSAHandler::private_encrypt(std::string  data, std::string & encrypted)
{
	int result = 0;
	if (m_prikey == 0) {
		printf("m_prikey can not be null\n");
		return -1;
	}
	RSA * rsa = m_prikey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int rsasize = RSA_size(rsa);//获取rsa->n 大小

		int exppadding = rsasize;
		int result = -1;
		char*  indata = (char*)data.data();
		int data_len = data.length();
		//.net Framework中提供的RSA算法规定，每次加密的字节数，不能超过密钥的长度值减去11,
		//而每次加密得到的密文长度，却恰恰是密钥的长度。
		/*
		1.RSA_PKCS1_PADDING 填充模式，最常用的模式

		要求:
		输入：必须 比 RSA 钥模长(modulus) 短至少11个字节, 也就是　RSA_size(rsa) – 11
		如果输入的明文过长，必须切割，　然后填充

		输出：和modulus一样长

		根据这个要求，对于512bit的密钥，　block length = 512/8 – 11 = 53 字节
		*/

		//if (data_len>exppadding - 11)
		exppadding = exppadding - 11;
		int b = 0;
		int s = data_len / (exppadding);
		if (data_len % (exppadding))
			s++;

		char* subdata = (char*)malloc(rsasize);
		char* encryptData = (char*)malloc(rsasize);
		encrypted.clear();
		result = 0;
		for (int i = 0; i < s; i++)//exppadding 每次的需要加密数据长度
		{
			memset(encryptData, 0x00, rsasize);
			memset(subdata, 0x00, rsasize);

			for (int j = 0;j<exppadding;j++)
			{
				if (i*exppadding + j>data_len) //超过原始数据长度跳出
					break;
				subdata[j] = indata[j + i*exppadding];
			}
			// exppadding : 待加密数据长度
			// smldata    : 待加密数据
			// smlencrypted : 存放密文数据
			//  rsa      : 存放密钥
			// 

			int ret = RSA_private_encrypt(exppadding, (const unsigned char*)subdata, (unsigned char*)encryptData, rsa, RSA_PKCS1_PADDING);
			if (ret>0)
			{
				encrypted.append(encryptData, rsasize);
				result += ret;
			}
			else {
				printf("%s\n", ERR_error_string(ERR_get_error(), NULL));

			}


		}
		if (encryptData) { free(encryptData); }
		if (subdata) { free(subdata); };
	}

	return result;
}



int RSAHandler::public_decrypt(std::string data, std::string & decrypted)
{
	int result = 0;
	if (m_pubkey == 0) {
		printf("m_pubkey can not be null\n");
		return -1;
	}
	RSA * rsa = m_pubkey;
	if (m_padding == RSA_PKCS1_PADDING) {
		int data_len = data.length();
		char * encdata = (char*)data.data();

		int rsasize = RSA_size(rsa);
		int s = data_len / (rsasize);
		if (data_len % (rsasize))
			s++;
		char* subdata = (char*)malloc(rsasize);
		char* decryptData = (char*)malloc(rsasize);
		decrypted.clear();

		for (int i = 0;i < s;i++)
		{
			for (int j = 0;j<rsasize;j++)
			{
				if (i*rsasize + j>data_len)
					break;
				subdata[j] = encdata[j + i*rsasize];
			}
			unsigned char smlencrypted[1024] = { 0 };
			int ret = RSA_public_decrypt(rsasize, (unsigned char *)subdata, (unsigned char*)decryptData, rsa, RSA_PKCS1_PADDING);
			if (ret>0)
			{
				decrypted.append(decryptData, ret);
				result += ret;
			}
		}
		if (decryptData) { free(decryptData); }
		if (subdata) { free(subdata); };
	}

	return result;
}
