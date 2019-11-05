#include "SM4Handler.h"
#include "common.h"


SM4Handler::SM4Handler()
{
}


SM4Handler::~SM4Handler()
{
}

int SM4Handler::do_encrypt(unsigned char * inBuf, int inBufLen, unsigned char * outBuf, int * outBufLen, int do_encrypt)
{
	EVP_CIPHER_CTX *ctx = 0;
	int tmplen = 0;
	int encLen = 0;
	int ret = 0;
	unsigned char iv[] = "1234567887654321";
	unsigned char key[16] = { 0xdd,0x13,0xdd,0x13,0x31,0x13,0x11,0x13,
		0x0f,0x13,0x11,0xcc,0xaa,0x13,0x11,0xaa };
	ctx = EVP_CIPHER_CTX_new();  //creates a cipher context.

	if (!EVP_CipherInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv, do_encrypt)) {
		ret = -1;goto err;
	}
	//int ret = EVP_Cipher(ctx,outBuf,inBuf,inBufLen);
	if (!EVP_CipherUpdate(ctx, outBuf, &tmplen, inBuf, inBufLen)) {
		ret = -1;goto err;
	}
	encLen = tmplen;
	if (!EVP_CipherFinal(ctx, outBuf + tmplen, &tmplen)) {
		ret = -1;goto err;
	}
	encLen += tmplen;
	*outBufLen = encLen;
	ret = 0;
err:
	EVP_CIPHER_CTX_free(ctx);
	return ret;

}