
#include <openssl/hmac.h>

/*
HMAC算法的一个典型应用是用在“挑战/响应”（Challenge/Response）身份认证中，认证流程如下：

(1) 先由客户端向服务器发出一个验证请求。

(2) 服务器接到此请求后生成一个随机数并通过网络传输给客户端（此为挑战）。

(3) 客户端将收到的随机数与自己的密钥进行HMAC-SHA1运算并得到一个结果作为认证证据传给服务器（此为响应）。

(4) 与此同时，服务器也使用该随机数与存储在服务器数据库中的该客户密钥进行HMAC-SHA1运算，如果服务器的运算结果与客户端传回的响应结果相同，则认为客户端是一个合法用户 。
*/
int HmacHash(const char * algo, 
		const char * key, unsigned int key_length, 
		const char * input, unsigned int input_length, 
		unsigned char * &output, unsigned int &output_length) {
	const EVP_MD * engine = NULL;
	if(strcmp("sha512", algo) == 0) {
		engine = EVP_sha512();
	}
	else if(strcmp("sha256", algo) == 0) {
		engine = EVP_sha256();
	}
	else if(strcmp("sha1", algo) == 0) {
		engine = EVP_sha1();
	}
	else if(strcmp("md5", algo) == 0) {
		engine = EVP_md5();
	}
	else if(strcmp("sha224", algo) == 0) {
		engine = EVP_sha224();
	}
	else if(strcmp("sha384", algo) == 0) {
		engine = EVP_sha384();
	}
	else {
		printf( "Algorithm %s is not supported by this program!\n",algo);
		return -1;
	}
 
	output = (unsigned char*)malloc(EVP_MAX_MD_SIZE);
	/*---------------------------------
	这块应该是相对通用的计算流程了
	--------------------------------*/
	HMAC_CTX * ctx = HMAC_CTX_new();

	HMAC_Init_ex(ctx, key, strlen(key), engine, NULL);
	HMAC_Update(ctx, (unsigned char*)input, strlen(input));	// input is OK; &input is WRONG !!!
 
	HMAC_Final(ctx, output, &output_length);
	HMAC_CTX_free(ctx);	
 
	return 0;
}
#include <string>
void testHMAC(){
	char key[] = "fasdkgjl;asdfjg;dsfjgasdsr";//secret key
	std::string data = "picsmall8ba5a744-f48d-4ba2-b93f-da17e7f52dff";//要加密传输的数据
 
	unsigned char * mac = NULL;
	unsigned int mac_length = 0;
	
	int ret = HmacHash("sha1", key, strlen(key), data.c_str(), data.length(), mac, mac_length);	
	
	if(0 == ret) {
		printf( "Algorithm HMAC encode succeeded!");
	}
	else {
		printf( "Algorithm HMAC encode failed!");
		return;
	}
 
	printf( "mac length: %d", mac_length);
	printf( "mac:");
	for(int i = 0; i < mac_length; i++) {
		printf("%-03x", (unsigned int)mac[i]);
	}
	
	
	if(mac) {
		free(mac);
		printf( "mac is freed!");
	}
	
}


