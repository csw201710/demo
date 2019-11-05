#include "RSAHandler.h"



void testRSA2() {
	RSAHandler obj;
	obj.generateRSA();
	std::string inData = "123123";
	srand(time(0));
	char buf[1024];
	for (int i = 0;i < sizeof(buf);i++) {
		buf[i] = rand();
		inData.clear();
		inData.append(buf, i);
		std::string enc;
		std::string dec;
#if 1
		obj.public_encrypt(inData, enc);
		//std::cout << "enc data: " << enc << std::endl;
		obj.private_decrypt(enc, dec);

		//std::cout << "dec data: " << dec << std::endl;
		if (memcmp(inData.data(), dec.data(), inData.length()) != 0) {
			printf("compare 1 not equal %d",i);
			return;
		}
		enc.clear();
		dec.clear();
		obj.private_encrypt(inData, enc);
		//std::cout << "enc data: " << enc << std::endl;
		obj.public_decrypt(enc, dec);
		//std::cout << "dec data: " << dec << std::endl;
		if (memcmp(inData.data(), dec.data(), inData.length()) != 0) {
			printf("compare 2 not equal %d", i);
			return;
		}
		std::cout << "equal " << std::endl;
#endif
	}




	//std::string signData;
	//obj.private_sign(inData, signData);
	//obj.public_verify(inData, signData);





}

int main(int argc, char* argv[]) {

	RSAHandler obj;

	testRSA2();
	return 0;
}

