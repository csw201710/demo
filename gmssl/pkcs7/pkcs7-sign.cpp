// https://wenku.baidu.com/view/691f51f90242a8956bece436.html
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/rand.h>
#include <openssl/rsa.h>

#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

#include <openssl/pkcs7.h>
#include <openssl/pkcs12.h>
#include <openssl/x509.h>

bool ParseP12File(const char* cert_file_path, char* cert_file_password,
                                     EVP_PKEY** pkey, X509** x509, STACK_OF(X509)** ca) {

  FILE* file = NULL;
#if defined(_WIN32) || defined(_WIN64)
  fopen_s(&file, cert_file_path, "rb");
#else
  file = fopen(String::FromUnicode(cert_file_path), "rb");
#endif  // defined(_WIN32) || defined(_WIN64)
  if (!file) {
    return false;
  }

  PKCS12* pkcs12 = d2i_PKCS12_fp(file, NULL);
  fclose (file);
  if (!pkcs12) {
    return false;
  }

  if (!PKCS12_parse(pkcs12, (const char*)cert_file_password, pkey, x509, ca)) {
    return false;
  }

  PKCS12_free(pkcs12);
  if (!pkey)
    return false;
  return true;
}

void pkcs7_test(){

	EVP_PKEY* pkey = NULL;
	X509* x509 = NULL;
	STACK_OF(X509)* ca = NULL;
	PKCS7* p7 = PKCS7_new();
	PKCS7_set_type(p7, NID_pkcs7_signed);//设置类型为NID_pkcs7_signed
	PKCS7_content_new(p7, NID_pkcs7_data);
	PKCS7_set_detached(p7, 0);
	//添加签名者信息，
	//x509：签名证书，pkey：签名者私钥。EVP_sha1()签名者摘要算法。
	if(!ParseP12File("foxit_all.pfx", "123456", &pkey, &x509, &ca)){
		return ;
	}

	PKCS7_SIGNER_INFO* info = PKCS7_add_signature(p7, x509, pkey, EVP_sha1());

	//添加签名者证书
	PKCS7_add_certificate(p7, x509);
	//添加签名者的CA证书链
	for (int i=0; i<sk_X509_num(ca); i++)
	{
	  PKCS7_add_certificate(p7, sk_X509_value(ca, i));
	}

	BIO* p7bio = PKCS7_dataInit(p7, NULL);
	BIO_write(p7bio, "How are you!", strlen("How are you!"));//加入原始数据，
	PKCS7_dataFinal(p7, p7bio); //处理数据。
	//转换为der编码输出
	  unsigned long derLen = i2d_PKCS7(p7, NULL);
	  unsigned char* der = reinterpret_cast<unsigned char*>(malloc(derLen));
	  memset(der, 0, derLen);
	unsigned char* dertmp = der;
	i2d_PKCS7(p7,&dertmp);
	PKCS7_free(p7);
	BIO_free(p7bio);

//解析P7签名的代码：

	//der编码转换为PKCS7结构体
	dertmp = der;
	p7 =d2i_PKCS7(NULL,(const unsigned char **)&dertmp,derLen);

	//解析出原始数据
	p7bio= PKCS7_dataDecode(p7,NULL,NULL,NULL);

	//从BIO中读取原始数据,将得到"How are you!"
	unsigned char src[1024]={0};
	int srcLen = BIO_read(p7bio,src,1024);

	//获得签名者信息stack
	STACK_OF(PKCS7_SIGNER_INFO) *sk = PKCS7_get_signer_info(p7);

	//获得签名者个数(本例只有1个)
	int signCount = sk_PKCS7_SIGNER_INFO_num(sk );

	for(int i=0;i <signCount ;i++)
	{
		//获得签名者信息
		PKCS7_SIGNER_INFO *signInfo = sk_PKCS7_SIGNER_INFO_value(sk,i);

		//获得签名者证书
		X509 *cert= PKCS7_cert_from_signer_info(p7,signInfo);

		//验证签名
		if(PKCS7_signatureVerify(p7bio,p7,signInfo,cert) != 1)
		{
			printf("signatureVerify Err\n");
		}else{
			printf("signatureVerify OK\n");
		}
	}
}
int main(int argc,char *argv[]){
	pkcs7_test();
	return 0;
}

