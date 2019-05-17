#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/rand.h>
#include <openssl/rsa.h>


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/aes.h>
#include <windows.h>

//#include <../rsa/rsa_locl.h>
#include <openssl/ossl_typ.h>

# include <openssl/x509.h>
# include <openssl/pem.h>
# include <openssl/bn.h>

#ifdef _DEBUG
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#else


#endif



#include <openssl/evp.h>






#include <iostream>  
using namespace std;  

#if 0
/************************************************************************/  
/*                                 main.cpp                             */  
/************************************************************************/  





X509* load_certfile(const char* certFile,int type)  
{  
    X509* x509 = NULL;  
    BIO* in = NULL;  
  
    if(certFile==NULL)  
        goto end;  
    in = BIO_new_file(certFile,"rb");  
    if(in==NULL)  
        goto end;  
    //将IO中数据以PEM格式读入到X509对象  
	if(type == 0){
		x509 = PEM_read_bio_X509(in,NULL,NULL,NULL);  
	}else{
		x509 = d2i_X509_bio(in, NULL);
	}
	
    if(x509== NULL)  
        goto end;  
end:  
    if(in)BIO_free(in);  
    return x509;  
}  

int CheckRootCert(X509 *x)
{
	X509* x509 = NULL;
	int CertLen;
	char CertBuff[4096];
	bool isbase64;
	BIO *bio;
	EVP_PKEY *pkey;
	
	
	x509 = load_certfile("D:\\xx\\xx_root.cer",1);
	//x509 = load_certfile("D:\\xx\\xxx\\xx\\root01.crt",0);
	if(x509==NULL)
	{
		printf("load_certfile error file :%s line:%d\n",__FILE__,__LINE__);
		return 0;
	}
	else
	{
		pkey = X509_get_pubkey(x509);
		if(pkey != NULL)
		{
			if(X509_verify(x,pkey) <= 0){
				
				fprintf(stderr,"X509_V_ERR_CERT_SIGNATURE_FAILURE  %s\n ",ERR_error_string(ERR_get_error(), NULL)) ;

				return 0;
			}
			printf("verify ok!\n");
			
		}
	}
	if (x509 != NULL) X509_free(x509);
	return 1;
}



void main(void)  
{  
	X509* x509 = NULL;
	ERR_load_crypto_strings();
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_algorithms();
	//x509 = load_certfile("D:\\xxx\\xx\\x\\client01.crt",0);
	x509 = load_certfile("d:\\xx\\server001.cer",1);
	CheckRootCert(x509);
	if (x509 != NULL) X509_free(x509);

}  

#else




#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>



#if 1
	#define CERT_PATH       "d:\\xx\\"
	#define ROOT_CERT       "xx_root.cer"
	#define CARD_CERT       "server001.cer"
#else
	#define CERT_PATH       "d:\\xx\\"
	#define ROOT_CERT       "root01.cer"
	#define CARD_CERT       "client01.cer"
#endif

#define GET_DEFAULT_CA_CERT(str) sprintf(str, "%s/%s", CERT_PATH, ROOT_CERT)
#define GET_CUSTOM_CERT(str, path, name) sprintf(str, "%s/%s", path, name)

#define MAX_LEGTH 4096


int my_load_cert(unsigned char *str, unsigned long *str_len,
              const char *verify_cert, const unsigned int cert_len)
{
    FILE *fp;
    fp = fopen(verify_cert, "rb");
    if ( NULL == fp)
    {
        fprintf(stderr, "fopen fail\n");
        return -1;
    }

    *str_len = fread(str, 1, cert_len, fp);
    fclose(fp);
    return 0;
}

X509 *der_to_x509(const unsigned char *der_str, unsigned int der_str_len)
{
    X509 *x509;
    x509 = d2i_X509(NULL, &der_str, der_str_len);
    if ( NULL == x509 )
    {
        fprintf(stderr, "d2i_X509 fail\n");

        return NULL;
    }
    return x509;
}
int x509_verify()
{
    int ret;
    char cert[MAX_LEGTH];

    unsigned char user_der[MAX_LEGTH];
    unsigned long user_der_len;
    X509 *user = NULL;

    unsigned char ca_der[MAX_LEGTH];
    unsigned long ca_der_len;
    X509 *ca = NULL;

    X509_STORE *ca_store = NULL;
    X509_STORE_CTX *ctx = NULL;
    STACK_OF(X509) *ca_stack = NULL;

    /* x509初始化 */
    ca_store = X509_STORE_new();
    ctx = X509_STORE_CTX_new();

    /* root ca*/
    GET_DEFAULT_CA_CERT(cert);
    /* 从文件中读取 */
    my_load_cert(ca_der, &ca_der_len, cert, MAX_LEGTH);
    /* DER编码转X509结构 */
    ca = der_to_x509(ca_der, ca_der_len);
    /* 加入证书存储区 */
    ret = X509_STORE_add_cert(ca_store, ca);
    if ( ret != 1 )
    {
        fprintf(stderr, "X509_STORE_add_cert fail, ret = %d\n", ret);
        goto EXIT;
    }

    /* 需要校验的证书 */
    GET_CUSTOM_CERT(cert, CERT_PATH, CARD_CERT);
    my_load_cert(user_der, &user_der_len, cert, MAX_LEGTH);
    user = der_to_x509(user_der, user_der_len);

    ret = X509_STORE_CTX_init(ctx, ca_store, user, ca_stack);
    if ( ret != 1 )
    {
        fprintf(stderr, "X509_STORE_CTX_init fail, ret = %d\n", ret);
        goto EXIT;
    }

    //openssl-1.0.1c/crypto/x509/x509_vfy.h
    ret = X509_verify_cert(ctx);
    if ( ret != 1 )
    {
        fprintf(stderr, "X509_verify_cert fail, ret = %d, error id = %d, %s\n", ret,
			ctx->error, X509_verify_cert_error_string(ctx->error));
        goto EXIT;
    }
    fprintf(stdout, "X509_verify_cert successful\n");
EXIT:
    X509_free(user);
    X509_free(ca);

    X509_STORE_CTX_cleanup(ctx);
    X509_STORE_CTX_free(ctx);

    X509_STORE_free(ca_store);

    return ret == 1 ? 0 : -1;
}

int main()
{
    OpenSSL_add_all_algorithms();
    x509_verify();
    return 0;
}

#endif

