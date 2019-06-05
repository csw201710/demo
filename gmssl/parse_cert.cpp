//https://bbs.csdn.net/topics/390822099
//https://blog.csdn.net/u013030990/article/details/37564205
//https://blog.csdn.net/yyfzy/article/details/46798965

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



int parseX509Items(unsigned char* pCert, int CertLen,std::map<string,string> & value)
{
	X509 *x509 = NULL;
	unsigned char *pTmp = pCert;
	x509 = d2i_X509(NULL, (const unsigned char **)&pTmp, CertLen);
	if (x509 == NULL)
		return -1;
	X509_NAME *subject = NULL;
	X509_NAME *issuer = NULL;
	
	//long Version = X509_get_version(x509);
	
	subject = X509_get_subject_name(x509);
	
	//issuer = X509_get_issuer_name(x509);
	
	int extCount=X509_get_ext_count(x509);
	for (int k = 0; k < extCount; ++k)
	{
		X509_EXTENSION* ex = X509_get_ext(x509, k);
		
		ASN1_OBJECT* obj = X509_EXTENSION_get_object(ex);
		char buff[1024];
		OBJ_obj2txt(buff, 1024, obj, 0);
		//自定义扩展项
		if (strcmp(buff, "1.2.156.xx.xx.xxx.xx") == 0)
		{
			ASN1_OCTET_STRING *pID=X509_EXTENSION_get_data(ex);
			if(pID->type== V_ASN1_OCTET_STRING)
				value["xx"] = string((char*)&pID->data[4]);
		}

	}

	int entriesNum = sk_X509_NAME_ENTRY_num(subject->entries);
	for (int i = 0; i < entriesNum; i++)
	{
		X509_NAME_ENTRY *name_entry = sk_X509_NAME_ENTRY_value(subject->entries, i);
		int Nid = OBJ_obj2nid(name_entry->object);
		switch (Nid)
		{
		case NID_countryName://国家
		{
			string str;
			char msginfo[512] = {0};
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
				
				
			}
			else
			{
				str = msginfo;
			}
			value["C"] = str;
		}
			break;
		case NID_stateOrProvinceName://省
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
				
			value["ST"] = str;
		}
			break;
		case NID_localityName://地区
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["L"] = str;
		}
			break;
		case NID_organizationName://组织
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["O"] = str;
		}
		
			break;
		case NID_organizationalUnitName://单位
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["OU"] = str;
		}

			break;
		case NID_commonName://通用名
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["CN"] = str;
		}
			break;
		case NID_pkcs9_emailAddress:   //Mail
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["E"] = str;
		}
			break;
		case NID_name:
		{
				string str;
				char msginfo[512] = { 0 };
				memcpy(msginfo, name_entry->value->data, name_entry->value->length);
				msginfo[name_entry->value->length] = '\0';
				if (V_ASN1_UTF8STRING == name_entry->value->type)
				{
					//utf8
					str = msginfo;
				}
				else
				{
					str = msginfo;
				}
				value["NID"] = str;
		}
		break;
		case NID_title:
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["title"] = str;
		}
		break;
		default:
		{
			string str;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				//utf8
				str = msginfo;
			}
			else
			{
				str = msginfo;
			}
			value["default"] = str;
		}
			break;
		}
	}
	ASN1_TIME* begT=X509_get_notBefore(x509);
	ASN1_TIME* EndT = X509_get_notAfter(x509);
	char begTBuff[128] = { 0 };
	int begTBuffLen = 128;
	char EndTBuff[128] = {0};
	int EndTBuffLen = 128;
	BIO *bio=NULL;
	int write = 0;

	bio = BIO_new(BIO_s_mem());
	if (bio) {
		if (ASN1_TIME_print(bio, begT))
			write = BIO_read(bio, begTBuff, begTBuffLen - 1);
		BIO_free(bio);
		bio = NULL;
	}
	begTBuff[write] = '\0';

	bio = BIO_new(BIO_s_mem());
	if (bio) {
		if (ASN1_TIME_print(bio, EndT))
			write = BIO_read(bio, EndTBuff, EndTBuffLen - 1);
		BIO_free(bio);
		bio = NULL;
	}
	EndTBuff[write] = '\0';
	value["beginTime"] = begTBuff;
	value["endTime"] = EndTBuff;

	X509_free(x509);
	return  0;;
}


