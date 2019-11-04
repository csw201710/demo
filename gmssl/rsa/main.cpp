#include "common.h"
#include "RSAHandler.h"
#include <iostream>


void testRSA() {
	RSAHandler obj;
	obj.generateRSA();
	std::string inData = "123123";
	srand(time(0));
	char buf[96];
	for (int i = 0;i < sizeof(buf);i++) {
		buf[i] = rand();
	}
	inData.append(buf, sizeof(buf));
	std::string enc;
	std::string dec;
#if 0
	obj.public_encrypt(inData, enc);
	//std::cout << "enc data: " << enc << std::endl;
	obj.private_decrypt(enc, dec);

	//std::cout << "dec data: " << dec << std::endl;
	if (memcmp(inData.data(),dec.data(),inData.length()) != 0) {
		printf("compare 1 not equal");
		return;
	}
	enc.clear();
	dec.clear();
	obj.private_encrypt(inData, enc);
	//std::cout << "enc data: " << enc << std::endl;
	obj.public_decrypt(enc, dec);
	//std::cout << "dec data: " << dec << std::endl;
	if (memcmp(inData.data(), dec.data(), inData.length()) != 0) {
		printf("compare 2 not equal");
		return;
	}
	printf("equal\n");
#endif


	std::string signData;
	obj.private_sign(inData, signData);
	obj.public_verify(inData, signData);





}



int main(int argc, char* argv[]) {
	ERR_load_crypto_strings();
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_algorithms();


	testRSA();
	return 0;
}