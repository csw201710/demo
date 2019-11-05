#include "common.h"
#include "SM3Handler.h"



SM3Handler::SM3Handler()
{
}


SM3Handler::~SM3Handler()
{
}

int SM3Handler::calcHash(char* inData, int inDataLen, unsigned char* outData, int * poutDataLen)
{
	EC_KEY *ec_key = NULL;
	int ret = 0;

	EVP_MD_CTX *md_ctx = NULL;
	unsigned char za[64];
	size_t zalen = sizeof(za);
	unsigned int outlen;

	if (inDataLen <= 0) {
		(*poutDataLen) = 0;
		return 0;
	}
#if 0
	ec_key = pubkeyToEC_KEY(pubkey, pubkeyLen);
	if (ec_key == 0) {
		ERROR("pubkeyToEC_KEY failed");
		ret = -1;
		goto err;
	}

	/*
	if(!SM2_compute_message_digest(EVP_sm3(), EVP_sm3(), inData,inDataLen, "1234567812345678", 16, outData, poutDataLen,ec_key)){
	ERROR("SM2_compute_message_digest failed");
	ret = -1;
	goto err;
	}
	*/

	if (!SM2_compute_id_digest(EVP_sm3(), "1234567812345678", 16, za, &zalen, ec_key)) {
		ERROR("SM2_compute_id_digest failed");
		ret = -1;goto err;
	}
#endif

	if (!(md_ctx = EVP_MD_CTX_new())) {
		printf("EVP_MD_CTX_new failed");
		ret = -1;goto err;
	}
	if (!EVP_DigestInit_ex(md_ctx, EVP_sm3(), NULL)) {
		printf("EVP_DigestInit_ex failed");
		ret = -1;goto err;
	}
#if 0
	if (!EVP_DigestUpdate(md_ctx, za, zalen)) {
		ERROR("EVP_DigestUpdate failed");
		ret = -1;goto err;
	}
#endif



	if (!EVP_DigestUpdate(md_ctx, inData, inDataLen)) {
		printf("EVP_DigestUpdate failed");
		ret = -1;goto err;
	}



	if (!EVP_DigestFinal_ex(md_ctx, outData, (unsigned int *)poutDataLen)) {
		printf("EVP_DigestFinal_ex failed");
		ret = -1;goto err;
	}


	ret = 0;
err:
	EVP_MD_CTX_free(md_ctx);
	EC_KEY_free(ec_key);
	return ret;
}
