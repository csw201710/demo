#include "SM2Handler.h"

#include "../crypto/sm2/sm2_lcl.h"

int SM2Handler::ECDSA_SIG_get_signdataBuf(const ECDSA_SIG * sig, unsigned char * buf, int * bufLen)
{
	/* (r, s) are pointed to (sig->r, sig->s), so dont free (r, s) */
	const BIGNUM *r = NULL;
	const BIGNUM *s = NULL;

	/* check arguments */
	if (!sig || !buf)
	{
		return -1;
	}
	if (*bufLen < 64) {
		printf("bufLen < 64\n");
		return -1;
	}
	memset(buf, 0x00, 64);
	/* check ECDSA_SIG
	* `ECDSA_SIG_get0() return void
	*/
	ECDSA_SIG_get0(sig, &r, &s);

	int num_r = BN_num_bytes(r);
	int num_s = BN_num_bytes(s);


	if (num_s > 32 || num_r > 32)
	{
		printf("num_s %d || num_r %d", num_s, num_r);
		return -1;
	}


	if (!BN_bn2bin(r, buf + (32 - num_r)))
	{
		printf("r BN_bn2bin failed");
		return -1;
	}
	if (!BN_bn2bin(s, buf + 32 + (32 - num_s)))
	{
		printf("s BN_bn2bin failed");
		return -1;
	}
	*bufLen = 64;
	return 0;
}

EC_KEY * SM2Handler::pubkeyToEC_KEY(unsigned char * pubkey, int len)
{

	EC_KEY *ret;
	BIGNUM *x = NULL;
	BIGNUM *y = NULL;
	if (!(ret = EC_KEY_new_by_curve_name(NID_sm2p256v1))) {
		printf("EC_KEY_new_by_curve_name failed \n");
		return NULL;
	}
	if (!(x = BN_bin2bn(pubkey, 0x20, NULL))) {
		printf("BN_bin2bn failed \n");
		goto end;
	}
	if (!(y = BN_bin2bn(pubkey + 0x20, 0x20, NULL))) {
		printf("BN_bin2bn failed \n");
		goto end;
	}
	if (!EC_KEY_set_public_key_affine_coordinates(ret, x, y)) {
		printf("EC_KEY_set_public_key_affine_coordinates failed \n");
		goto end;
	}
end:
	BN_free(x);
	BN_free(y);
	return ret;

}

EC_KEY * SM2Handler::prikeyTOEC_KEY(unsigned char * prikey, int len)
{
	int ret = -1;
	BIGNUM *d = NULL;
	EC_KEY *ec_key = 0;
	ec_key = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	if (ec_key == 0) {
		printf("EC_KEY_new_by_curve_name failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		goto end;
	}
	int BitLen = EC_GROUP_get_degree(EC_KEY_get0_group(ec_key));	
	if (len != 32) {
		printf("pri key is invalid\n");
		goto end;
	}
	if (!(d = BN_bin2bn(prikey, len, NULL))) {
		printf("BN_bin2bn failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		goto end;
	}

	if (!EC_KEY_set_private_key(ec_key, d)) {
		printf("EC_KEY_set_private_key  failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		goto end;
	}
	ret = 0;
end:
	BN_clear_free(d);
	if (ret == 0) {
		return ec_key;
	}
	EC_KEY_free(ec_key);
	return 0;
	
}

int SM2Handler::getPrivateKeyFromEC_KEY(EC_KEY * ec_key, unsigned char* buf, int *pBufLen)
{
	const BIGNUM *d;

	int BitLen = EC_GROUP_get_degree(EC_KEY_get0_group(ec_key));
	if (BitLen % 8 != 0 || BitLen > 512) {
		printf("invalid ec_key\n");
		return -1;
	}
	if (!(d = EC_KEY_get0_private_key(ec_key))) {
		printf("invalid ec_key \n");
		return -1;
	}

	int size = BitLen / 8;

	if (*pBufLen < size) {
		printf("buf is too small\n");
		return -1;
	}
	memset(buf, 0x00, size);
	if (!BN_bn2bin(d, buf + size - BN_num_bytes(d))) {
		printf("BN_bn2bin failed\n");
		return -1;
	}
	*pBufLen = size;
	return 0;
}

int SM2Handler::getPublicKeyFromEC_KEY(EC_KEY * ec_key, unsigned char * buf, int * pBufLen)
{
	int ret = -1;
	BIGNUM *x = NULL;
	BIGNUM *y = NULL;
	BN_CTX *bn_ctx = NULL;
	int BitLen;

	const EC_GROUP *group = EC_KEY_get0_group(ec_key);
	const EC_POINT *point = EC_KEY_get0_public_key(ec_key);

	BitLen = EC_GROUP_get_degree(group);
	if ( BitLen > 512) {
		printf("invalid ec_key\n");
		goto end;
	}
	if (*pBufLen < 64) {
		printf("buf is too small\n");
		goto end;
	}

	x = BN_new();
	y = BN_new();
	bn_ctx = BN_CTX_new();
	if (!x || !y || !bn_ctx) {
		printf("BN new failed\n");
		goto end;
	}

	if (EC_METHOD_get_field_type(EC_GROUP_method_of(group)) == NID_X9_62_prime_field) {
		if (!EC_POINT_get_affine_coordinates_GFp(group, point, x, y, bn_ctx)) {
			printf("EC_POINT_get_affine_coordinates_GFp failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
			goto end;
		}
	}
	else {
		if (!EC_POINT_get_affine_coordinates_GF2m(group, point, x, y, bn_ctx)) {
			printf("EC_POINT_get_affine_coordinates_GF2m failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
			goto end;
		}
	}
	

	if (!BN_bn2bin(x, buf + 32 - BN_num_bytes(x))) {
		printf("BN_bn2bin failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		goto end;
	}
	if (!BN_bn2bin(y, buf + 64 - BN_num_bytes(y))) {
		printf("BN_bn2bin failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		goto end;
	}

	*pBufLen = 64;
	ret = 0;
end:
	BN_free(x);
	BN_free(y);
	BN_CTX_free(bn_ctx);
	return ret;
}

SM2Handler::SM2Handler()
{
}


SM2Handler::~SM2Handler()
{
}

int SM2Handler::generateKey()
{
	EC_KEY *key = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	if (!EC_KEY_generate_key(key)) {
		EC_KEY_free(key);
		return -1;
	}
	m_prikey = m_pubkey = key;
	return 0;
}

int SM2Handler::public_encrypt(std::string & data, std::string & encrypt) {
	return _encrypt(data, encrypt, 0);
}
int SM2Handler::private_encrypt(std::string & data, std::string & encrypt) {
	return _encrypt(data, encrypt, 1);
}
int SM2Handler::public_decrypt(std::string & data, std::string & decrypt) {
	return _decrypt(data, decrypt, 0);
}
int SM2Handler::private_decrypt(std::string & data, std::string & decrypt) {
	return _decrypt(data, decrypt, 1);
}
int SM2Handler::_encrypt(std::string & data, std::string & encrypt, int isprivate)
{
	const unsigned char* inData = (const unsigned char*)data.data();
//	const unsigned char* p;
	int inDataLen = data.length();
	int ret;
	unsigned char *out;
	size_t outlen;
	EC_KEY *ec_key;
	int clen = 0;

	if (isprivate == 1) {
		ec_key = m_prikey;
	}
	else {
		ec_key = m_pubkey;
	}
	
	
	ret = SM2_encrypt(NID_sm3, inData, inDataLen, 0, &outlen, ec_key);
	if (ret != 1) {
		printf("SM2_encrypt failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		return -1;
	}
	out = (unsigned char*)calloc(outlen, sizeof(char));
	if (out == 0) { printf("calloc failed"); return -1; }
	ret = SM2_encrypt(NID_sm3, inData, inDataLen, out, &outlen, ec_key);
	if (ret != 1) {
		printf("SM2_encrypt failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		free(out);
		return -1;
	}
	encrypt.clear();
	encrypt.append((char*)out, outlen);
	free(out);
	return 0;
}

int SM2Handler::_decrypt(std::string & data, std::string & decrypt,int isprivate)
{
	const unsigned char* inData = (const unsigned char*)data.data();
	size_t inDataLen = data.length();
	unsigned char* p;
	int ret;
	EC_KEY *ec_key;
	unsigned char *out;
	size_t outlen = 0;

	if (isprivate == 1) {
		ec_key = m_prikey;
	}
	else {
		ec_key = m_pubkey;
	}

	ret = SM2_decrypt(NID_sm3, inData, inDataLen, 0, &outlen, ec_key);
	if (ret != 1) {
		printf("SM2_decrypt failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		return -1;
	}
	
	out = (unsigned char*)calloc(outlen, sizeof(char));
	if (out == 0) { printf("calloc failed"); return -1; }


	ret = SM2_decrypt(NID_sm3, inData, inDataLen, out, &outlen, ec_key);
	if (ret != 1) {
		printf("SM2_decrypt failed,%s\n ", ERR_error_string(ERR_get_error(), 0));
		free(out);
		return -1;
	}

	decrypt.clear();
	decrypt.append((char*)out, outlen);
	free(out);
	return 0;
}

int SM2Handler::private_sign(std::string & data, std::string & signData)
{
	int ret;
	ECDSA_SIG * es_sign = 0;
	char outSignData[128] = { 0 };
	int outSignDataLen = sizeof(outSignData);

	EC_KEY *ec_key = m_prikey;

	if (ec_key == 0) {
		printf("m_prikey can not be null\n");
		return -1;
	}

	es_sign = SM2_do_sign((const unsigned char*)data.data(), data.length(), ec_key);
	if (es_sign == 0)
	{
		printf("SM2_do_sign failed!");
		ret = -1; goto err;
	}
	ret = ECDSA_SIG_get_signdataBuf(es_sign, (unsigned char*)outSignData, &outSignDataLen);
	if (ret != 0) {
		printf("ECDSA_SIG_get_signdataBuf failed!");
		ret = -1; goto err;
	}
	signData.clear();
	signData.append(outSignData, outSignDataLen);
	ret = 0;
err:
	if (es_sign) ECDSA_SIG_free(es_sign);
	return ret;
}

int SM2Handler::public_verify(std::string & data, std::string & signData)
{
	int ret;
	ECDSA_SIG *ecdSig = 0;
	BIGNUM* r = 0, *s = 0;
	const unsigned char* dgst = (const  unsigned char*)data.data();
	int dgstLen = data.length();

	const  unsigned char* sig = (const  unsigned char*)signData.data();
	int sigLen = signData.length();
	
	EC_KEY *ec_key = m_pubkey;

	if (ec_key == 0) {
		printf("m_pubkey can not be null\n");
		return -1;
	}

	if (sigLen < 64) {
		printf("sigature data sigLen < 64");
		ret = -1;
		goto err;
	}

	ecdSig = ECDSA_SIG_new();
	if (sig == NULL)
	{
		printf("ECDSA_SIG_new error.");
		ret = -1;
		goto err;
	}
	r = BN_new();
	s = BN_new();

	ECDSA_SIG_set0(ecdSig, r, s);

	if (!BN_bin2bn(sig, 32, r)) {
		printf("BN_bin2bn r error.");
		ret = -1;
		goto err;
	}
	if (!BN_bin2bn(sig + 32, 32, s)) {
		printf("BN_bin2bn s error.");
		ret = -1;
		goto err;
	}
	/*
	derlen = i2d_ECDSA_SIG(ecdSig, &der);
	if (derlen != sigLen || memcmp(sig, der, derlen)) {
	ERROR("sigautre data is not correct");
	ret = -1;
	goto err;
	}

	*/

	ret = SM2_do_verify(dgst, dgstLen, ecdSig, ec_key);
	if (ret != 1) {
		unsigned long ulErr = ERR_get_error();
		const char* errStr = ERR_lib_error_string(ulErr);
		const char* errFunc = ERR_func_error_string(ulErr);
		printf("%s\n ", ERR_error_string(ERR_get_error(), 0));
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	/*
	if (derlen > 0) {
	OPENSSL_cleanse(der, derlen);
	OPENSSL_free(der);
	}
	*/
	if(ecdSig) ECDSA_SIG_free(ecdSig);

	return ret;
}
