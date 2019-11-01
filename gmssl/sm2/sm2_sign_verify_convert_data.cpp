
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <deque>
#include <pthread.h>
#include <algorithm>



#include <openssl/bio.h>
#include <openssl/asn1.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/sm2.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/md5.h>


#define ERROR(...) \
do{ \
    fprintf(stderr, "[ERROR  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
    fprintf(stderr, "\33[34m"); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\33[0m\n"); \
}while(0);
#define INFO(...) \
do{ \
    fprintf(stderr, "[INFO  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
}while(0);

#define TESTLOG_DATA(d, l)\
fprintf(stdout,"======" #d " len:%d"  "  start ======\n",l); \
do\
{\
	int i;\
  unsigned char* t = (unsigned char*) d;\
	for(i=0;i<l;i++)\
	{\
		if((i+1) % 16) \
			printf("%02X ", t[i]);\
		else\
			printf("%02X\n", t[i]);\
	}\
	if(i % 16) printf("\n");\
}\
while(0); \
fprintf(stdout,"======" #d "  end ======\n");

static int ECDSA_SIG_get_signdataBuf(const ECDSA_SIG *sig, unsigned char *buf,int *bufLen)
{
#if 1
    /* (r, s) are pointed to (sig->r, sig->s), so dont free (r, s) */
    const BIGNUM *r = NULL;
    const BIGNUM *s = NULL;

    /* check arguments */
    if (!sig || !buf)
    {
        return -1;
    }
    if(*bufLen < 64){
      ERROR("bufLen < 64");
      return -1;
    }
    memset(buf, 0x00, 64);
    /* check ECDSA_SIG
     * `ECDSA_SIG_get0() return void
     */
    ECDSA_SIG_get0(sig, &r, &s);

    int num_r = BN_num_bytes(r);
    int num_s = BN_num_bytes(s);


	if( num_s > 32 || num_r > 32)
  {
    ERROR("num_s %d || num_r %d",num_s, num_r);
		return -1;
	}


    if (!BN_bn2bin(r, buf + (32-num_r)))
    {
        ERROR("r BN_bn2bin failed");
        return -1;
    }
    if (!BN_bn2bin(s, buf + 32 + (32-num_s)))
    {
        ERROR("s BN_bn2bin failed");
        return -1;
    }
	  *bufLen= 64;
    return 0;
#else


#endif

}

const char * g_pri_key=
  "-----BEGIN PRIVATE KEY-----\n"
  "-----END PRIVATE KEY-----\n";

  unsigned  char g_pub_key[]={
  };

  static EC_KEY* pubkeyToEC_KEY(unsigned char * pubkey, int len){

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

static int load_private_key(const char *keyfile, const  char *passwd, EVP_PKEY **pri_key){
   BIO *bp = NULL;

   #if 1
       bp = BIO_new_file(keyfile, "rb");
       if (NULL == bp)
       {
           ERROR("open_private_key bio file new error!\n");
           return -1;
       }
   #else
     bp = BIO_new(BIO_s_mem());
     BIO_write(bp, g_pri_key, strlen(g_pri_key));
     BIO_seek(bp,0);
   #endif

    EVP_PKEY *key = PEM_read_bio_PrivateKey(bp, pri_key, NULL, (void *)passwd);
    if (key == NULL)
    {
        ERROR("PEM_read_bio_PrivateKey failed!\n");
        BIO_free(bp);

        return -1;
    }
    BIO_free(bp);
	return 0;
}



int SignData(const char* plainData, int plainDataLen, char* outSignData,int* outSignDataLen){
    int ret = 0;
	EC_KEY *ec = 0 ;
	ECDSA_SIG *es_sign = 0;
    EVP_PKEY *pri_key = 0;
  ret = load_private_key("1.key", 0, &pri_key);
	if(ret !=0){
		ERROR("load privatekey failed");
		ret = -1; goto err;
	}


    ec = EVP_PKEY_get0_EC_KEY(pri_key);
	if(ec == 0){
		ERROR("get ec key failed");
		ret = -1; goto err;
	}
    es_sign = SM2_do_sign((const unsigned char*)plainData, plainDataLen, ec);
    if (es_sign == 0)
    {
        ERROR("SM2_do_sign failed!");
        ret = -1; goto err;
    }
    ret = ECDSA_SIG_get_signdataBuf(es_sign, (unsigned char* )outSignData,outSignDataLen);
	if( ret != 0){
		ERROR("ECDSA_SIG_get_signdataBuf failed!");
		ret = -1; goto err;
	}
	ret = 0;
err:
	if(pri_key) EVP_PKEY_free(pri_key);
	if(es_sign) ECDSA_SIG_free(es_sign);
	return ret;
}



int VerifyData(unsigned char * dgst,int dgstLen, unsigned char* sig,int sigLen)
{

  EC_KEY *ec_key = NULL;
  int ret = 0;
  ECDSA_SIG *ecdSig = 0;
  BIGNUM* r = 0, *s = 0;
  unsigned char *der = NULL;
  int derlen = 0;

  ec_key = pubkeyToEC_KEY(g_pub_key, 64);
  if(ec_key == 0){
    ERROR("pubkeyToEC_KEY failed");
    ret = -1;
    goto err;
  }

  if(sigLen < 64){
    ERROR("sigature data sigLen < 64");
    ret = -1;
    goto err;
  }

  ecdSig = ECDSA_SIG_new();
  if(ecdSig == NULL)
  {
  	ERROR("ECDSA_SIG_new error.");
  	ret = -1;
  	goto err;
  }
/*
  r = BN_new();
  if(r == 0){
    ERROR("BN_new s error.");
  	ret = -1;
  	goto err;
  }

  s = BN_new();
  if(s == 0){
    ERROR("BN_new r error.");
    ret = -1;
    goto err;
  }

  */



  //if (!BN_bin2bn(sig, 32, r))
  if (!(r = BN_bin2bn(sig, 32, NULL))) {
  	ERROR("BN_bin2bn r error.");
  	ret = -1;
  	goto err;
  }
  //if (!BN_bin2bn(sig + 32, 32, s))
  if(!(s = BN_bin2bn(sig+32, 32, NULL))){
  	ERROR("BN_bin2bn s error.");
  	ret = -1;
  	goto err;
  }
  if (!ECDSA_SIG_set0(ecdSig, r, s)) {
    ERROR("ECDSA_SIG_set0 error.");
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
  if(ret != 1){
    unsigned long ulErr = ERR_get_error();
    const char* errStr = ERR_lib_error_string(ulErr);
    const char* errFunc = ERR_func_error_string(ulErr);
    //ERR_error_string(ERR_get_error()
    ERROR("SM2_do_verify failed,ulErr:%d errStr:[%s] errFunc:[%s]",ulErr,errStr, errFunc);
    //LOG::DEBUGHEXLOG((char*)dgst,dgstLen,"SM2_do_verify [indata]");
    //LOG::DEBUGHEXLOG((char*)sig,sigLen,"SM2_do_verify [sigData]");
    ret = -1;
    goto err;
  }

  ret = 0;
err:
  if (derlen > 0) {
    OPENSSL_cleanse(der, derlen);
    OPENSSL_free(der);
  }
  ECDSA_SIG_free(ecdSig);
  EC_KEY_free(ec_key);
  	//BN_free(r);
	//BN_free(s);
  return ret;
}


int SignVerifyData(const char* plainData, int plainDataLen){
    int ret = 0;
    //unsigned char pubkey[64];
    //int pubkeyLen = sizeof(pubkey);
	EC_KEY *ec = 0 ;
  EC_KEY *ec_pubkey = 0 ;
	ECDSA_SIG *es_sign = 0;
    EVP_PKEY *pri_key = 0;
  ret = load_private_key("1.key", 0, &pri_key);
	if(ret !=0){
		ERROR("load privatekey failed");
		ret = -1; goto err;
	}
  ec_pubkey = pubkeyToEC_KEY(g_pub_key, 64);
  if(ec_pubkey == 0){
    ERROR("pubkeyToEC_KEY failed");
    ret = -1;
    goto err;
  }


    ec = EVP_PKEY_get0_EC_KEY(pri_key);
	if(ec == 0){
		ERROR("get ec key failed");
		ret = -1; goto err;
	}
    es_sign = SM2_do_sign((const unsigned char*)plainData, plainDataLen, ec);
    if (es_sign == 0)
    {
        ERROR("SM2_do_sign failed!");
        ret = -1; goto err;
    }

    ret = SM2_do_verify((const unsigned char*)plainData, plainDataLen, es_sign, ec_pubkey);
    if(ret != 1){
      unsigned long ulErr = ERR_get_error();
      const char* errStr = ERR_lib_error_string(ulErr);
      const char* errFunc = ERR_func_error_string(ulErr);
      //ERR_error_string(ERR_get_error()
      ERROR("SM2_do_verify failed,ulErr:%d errStr:[%s] errFunc:[%s]",ulErr,errStr, errFunc);
      //LOG::DEBUGHEXLOG((char*)dgst,dgstLen,"SM2_do_verify [indata]");
      //LOG::DEBUGHEXLOG((char*)sig,sigLen,"SM2_do_verify [sigData]");
      ret = -1;
      goto err;
    }

	ret = 0;
err:
	if(ec_pubkey) EC_KEY_free(ec_pubkey);
  if(pri_key) EVP_PKEY_free(pri_key);
	if(es_sign) ECDSA_SIG_free(es_sign);
	return ret;
}



void testSignVerifyData(){
  unsigned char inData[]={ 0x11
  } ;
  int i=0;
  unsigned char signData[128]={0};
  int signDataLen = sizeof(signData);
  int ret;
  for(int i=0;i<1000;i++){
#if 1
    ret = SignData((char*)inData, sizeof(inData), (char*)signData, &signDataLen);
    if(ret != 0){
      ERROR("[test] SignData failed, i: %d ", i);
      return ;
    }

    ret = VerifyData(inData, sizeof(inData),signData, signDataLen);
    if(ret != 0){
      ERROR("[test] VerifyData failed, i: %d ", i);
      return ;
    }
#else
    ret = SignVerifyData((char*)inData, sizeof(inData));
    if(ret != 0){
      ERROR("[test] SignVerifyData failed, i: %d ", i);
      return ;
    }
#endif
  }

  INFO("%s ok",__func__);
}




int main(int argc,char** argv){
  OpenSSL_add_all_ciphers();
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

  testSignVerifyData();
  return 0;
}


