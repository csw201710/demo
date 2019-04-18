#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/objects.h>


typedef struct ECCrefPublicKey_st {
	unsigned int bits;
	unsigned char x[64];
	unsigned char y[64];
} ECCrefPublicKey;

static int EC_KEY_set_ECCrefPublicKey(EC_KEY *ec_key, const ECCrefPublicKey *ref)
{
	int ret = 0;
	BIGNUM *x = NULL;
	BIGNUM *y = NULL;
	int nbytes;

	/* check arguments */
	if (!ec_key || !ref) {
		return 0;
	}
	if (ref->bits != EC_GROUP_get_degree(EC_KEY_get0_group(ec_key))) {
		return 0;
	}

	/* ECCrefPublicKey ==> EC_KEY */
	nbytes = (ref->bits + 7)/8;

	if (!(x = BN_bin2bn(ref->x, nbytes, NULL))) {
		
		goto end;
	}
	if (!(y = BN_bin2bn(ref->y, nbytes, NULL))) {
		
		goto end;
	}
	if (!EC_KEY_set_public_key_affine_coordinates(ec_key, x, y)) {
		
		goto end;
	}

	ret = 1;
end:
	BN_free(x);
	BN_free(y);
	return ret;
}

EC_KEY *EC_KEY_new_from_ECCrefPublicKey(const ECCrefPublicKey *ref)
{
	EC_KEY *ret;

	if (!ref) {
		return NULL;
	}

	if (!(ret = EC_KEY_new_by_curve_name(NID_sm2p256v1))) {
		return NULL;
	}

	if (!EC_KEY_set_ECCrefPublicKey(ret, ref)) {
		EC_KEY_free(ret);
		return NULL;
	}

	return ret;
}

static int test_evp_pkey_verify_sign(EVP_PKEY *pkey,unsigned char* buf,size_t buflen ,unsigned char *signbuf, size_t signlen)
{
	int ret = 0;
	EVP_PKEY_CTX *pkctx = NULL;

	if (!(pkctx = EVP_PKEY_CTX_new(pkey, NULL))) {
		ERR_print_errors_fp(stderr);
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_verify_init(pkctx)) {
		ERR_print_errors_fp(stderr);
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}

	if (!EVP_PKEY_verify(pkctx, signbuf, signlen, buf, buflen)) {
		fprintf(stderr, "error: %s %d\n", __FILE__, __LINE__);
		goto end;
	}
	else
	{
		printf("verify sign OK\n");
	}

	ret = 1;
end:
	EVP_PKEY_CTX_free(pkctx);
	return ret;
}


void testEC_Verifity(){
	int ret = 0;
	EC_KEY *pk = NULL;
	EVP_PKEY * evp_key = NULL;
	
	
	//TODO
	ECCrefPublicKey pkref;
	unsigned char buf[1024];
	unsigned char signBuf[1024];
	size_t bufLen = sizeof(buf) ;
	size_t signBufLen = sizeof(signBuf);
	
	if (!(pk = EC_KEY_new_from_ECCrefPublicKey(&pkref))){
		goto end;
	}
	
	evp_key = EVP_PKEY_new();
	ret = EVP_PKEY_set1_EC_KEY(evp_key, pk);

	
	if(test_evp_pkey_verify_sign(evp_key,buf, bufLen, signBuf, signBufLen) != 1){
		goto end;
	}
	
end:
	EVP_PKEY_free(evp_key);
	EC_KEY_free(pk);
	
	return;
	
}


