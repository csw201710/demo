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
#include <../crypto/include/internal/x509_int.h>
int GetECCCertPubKey(unsigned char *cert, int certLen, unsigned char *pubkey, int *pubkeyLen)
{
    const unsigned char *pbcert = NULL;
    X509 *x = NULL;
    ASN1_BIT_STRING *ec_public_key = NULL;
    unsigned char *p;
    int len;

    pbcert = cert;
    x = d2i_X509(NULL, &pbcert, certLen);
    if (x == NULL)
    {
        BIO *mem = BIO_new(BIO_s_mem());
        BIO_write(mem, pbcert, certLen);
        BIO_seek(mem, 0);
        x = PEM_read_bio_X509(mem, &x, 0, 0);
        BIO_free(mem);
        if (x == 0)
        {
            return -1;
        }


    }

    ec_public_key = X509_get0_pubkey_bitstr(x);
    if (ec_public_key == NULL)
    {
        X509_free(x);
        return -1;
    }

    len = ASN1_STRING_length(ec_public_key);
    if (len <= 0)
    {
        X509_free(x);
        return -1;
    }

    p = ASN1_STRING_data(ec_public_key);
    if (p == NULL)
    {
        X509_free(x);
        return -1;
    }
    if (len == 96)
    {
        memcpy(pubkey, p, len);
        *pubkeyLen = len;
    }
    else
    {
        memcpy(pubkey, p + 1, len - 1);
        *pubkeyLen = len - 1;
    }
    X509_free(x);
    return 0;
}




#include <openssl/sm2.h>


int My_SM2_do_verify(unsigned char *pubkey, int pubkeyLen, unsigned char *dgst, int dgstLen,
                     unsigned char *sig, int sigLen)
{
    EC_KEY *ec_key = NULL;
    int ret = 0;
    ECDSA_SIG *ecdSig = 0;
    BIGNUM *r = 0, *s = 0;
    unsigned char *der = NULL;
    int derlen = -1;

    ec_key = pubkeyToEC_KEY(pubkey, pubkeyLen);
    if (ec_key == 0)
    {
        printf("pubkeyToEC_KEY failed");
        ret = -1;
        goto err;
    }
    if (sigLen < 64)
    {
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

    if (!BN_bin2bn(sig, 32, r))
    {
        printf("BN_bin2bn r error.");
        ret = -1;
        goto err;
    }
    if (!BN_bin2bn(sig + 32, 32, s))
    {
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
    if (ret != 1)
    {
        printf("SM2_do_verify failed");
        ret = -1;
        goto err;
    }

    ret = 0;
err:
    if (derlen > 0)
    {
        OPENSSL_cleanse(der, derlen);
        OPENSSL_free(der);
    }
    ECDSA_SIG_free(ecdSig);
    EC_KEY_free(ec_key);

    return ret;
}

int load_private_key(const char *keyfile, const  char *passwd, EVP_PKEY **pri_key)
{


    BIO *bp = NULL;
    bp = BIO_new_file(keyfile, "rb");
    if (NULL == bp)
    {
        printf("open_private_key bio file new error!\n");
        return -1;
    }

    EVP_PKEY *key = PEM_read_bio_PrivateKey(bp, pri_key, NULL, (void *)passwd);
    if (key == NULL)
    {
        printf("open_private_key failed to PEM_read_bio_RSAPrivateKey!\n");
        BIO_free(bp);

        return -1;
    }

    return 0;
}


static int ECDSA_SIG_get_signdataBuf(const ECDSA_SIG *sig, unsigned char *buf,int *bufLen)
{
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
}



int getfileBuf(char *filename, unsigned char *buf, int *bufLen)
{
    int i = 0;
    int has_read = 0;
    BIO *file_io = BIO_new_file(filename, "rb");
    while (1)
    {
        i = BIO_read(file_io, buf + has_read, 1024);
        if (i == 0)
        {
            break;
        }
        if (i < 0)
        {
            printf("BIO_read error");
            return -1;
        }
        has_read += i;
    }
    *bufLen = has_read;
    BIO_free(file_io);
    return 0;
}

void testSm2()
{
    int ret = 0;
    unsigned char pubkey[64];
    int pubkeyLen = sizeof(pubkey);
    unsigned char cert[2048] = {0};
    int certLen = sizeof(cert);
    unsigned char plainData[1024];
    int plainDataLen = 10;
    unsigned char signedData[1024];
    int signedDataLen = 10;

    for (int i = 0; i < plainDataLen; i++)
    {
        plainData[i] = (unsigned char)i;
    }


    EVP_PKEY *pri_key = 0;
    ret = load_private_key("D:\\xxxx\\client01.key", 0, &pri_key);
	if(ret !=0){
		goto err;
	}


    EC_KEY *ec = EVP_PKEY_get0_EC_KEY(pri_key);
	if(ec == 0){
		goto err;
	}
    ECDSA_SIG *es_sign = SM2_do_sign(plainData, plainDataLen, ec);
    if (es_sign == 0)
    {
        printf("SM2_do_sign failed!");
        return ;
    }
    ret = ECDSA_SIG_get_signdataBuf(es_sign, signedData,&signedDataLen);
	if( ret != 0){
		goto err;
	}

    ret = getfileBuf("D:\\xxxx\\client01.crt", cert, &certLen);
	if( ret != 0){
		goto err;
	}

    printf("use soft sm2 verify");
    if (GetECCCertPubKey(cert, certLen, pubkey, &pubkeyLen) != 0)
    {

        printf("GetECCCertPubKey failed!");

        goto err;
    }
    ret  = My_SM2_do_verify(pubkey, 64, plainData, (int)plainDataLen, signedData, (int)signedDataLen);
    if (ret != 0)
    {

        printf("My_SM2_do_verify failed ret:0x%x", ret);

        goto err;
    }
    printf("My_SM2_do_verify ok ");
err:
    return;
}













