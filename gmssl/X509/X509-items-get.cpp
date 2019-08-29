

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Ws2_32.lib")
#include <iostream>
#include <string.h>
#include <string>
#include <map>
using namespace std;
#include <sys/stat.h>


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

#ifdef _DEBUG
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"libssl.lib")
#else


#endif

#include <time.h>


static time_t ASN1_GetTimeT(ASN1_TIME* time)
{
    struct tm t;
    const char* str = (const char*) time->data;
    size_t i = 0;

    memset(&t, 0, sizeof(t));

    if (time->type == V_ASN1_UTCTIME) {/* two digit year */
        t.tm_year = (str[i++] - '0') * 10;
        t.tm_year += (str[i++] - '0');
        if (t.tm_year < 70)
            t.tm_year += 100;
    } else if (time->type == V_ASN1_GENERALIZEDTIME) {/* four digit year */
        t.tm_year = (str[i++] - '0') * 1000;
        t.tm_year+= (str[i++] - '0') * 100;
        t.tm_year+= (str[i++] - '0') * 10;
        t.tm_year+= (str[i++] - '0');
        t.tm_year -= 1900;
    }
    t.tm_mon  = (str[i++] - '0') * 10;
    t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
    t.tm_mday = (str[i++] - '0') * 10;
    t.tm_mday+= (str[i++] - '0');
    t.tm_hour = (str[i++] - '0') * 10;
    t.tm_hour+= (str[i++] - '0');
    t.tm_min  = (str[i++] - '0') * 10;
    t.tm_min += (str[i++] - '0');
    t.tm_sec  = (str[i++] - '0') * 10;
    t.tm_sec += (str[i++] - '0');

    /* Note: we did not adjust the time based on time zone information */
    return mktime(&t);
}

int GetX509Items(unsigned char* pCert, int CertLen,std::map<string,string> & value)
{
	X509 *usrCert = NULL;
	unsigned char *pTmp = pCert;
	usrCert = d2i_X509(NULL, (const unsigned char **)&pTmp, CertLen);
	if (usrCert == NULL)
		return -1;
	X509_NAME *subject = NULL;
	//X509_NAME *issuer = NULL;
	long Version = X509_get_version(usrCert);
	subject = X509_get_subject_name(usrCert);
	int extCount=X509_get_ext_count(usrCert);
	for (int k = 0; k < extCount; ++k)
	{
		X509_EXTENSION* ex = X509_get_ext(usrCert, k);
		
		ASN1_OBJECT* obj = X509_EXTENSION_get_object(ex);
		char buff[1024];
		OBJ_obj2txt(buff, 1024, obj, 0);
		if (strcmp(buff, "1.2.156.10260.4.1.1") == 0)
		{
			ASN1_OCTET_STRING *pID=X509_EXTENSION_get_data(ex);
			if(pID->type== V_ASN1_OCTET_STRING)
				value["ID"] = string((char*)&pID->data[4]);
				//cout <<"身份证:"<< &pID->data[4] << endl;
		}
		
		/*
		BUF_MEM *bptr = NULL;
		char *buf = NULL;
		BIO *bio = BIO_new(BIO_s_mem());
		if (!X509V3_EXT_print(bio, ex, 0, 0)) {
			fprintf(stderr, "Error in reading extensions");
		}
		BIO_flush(bio);
		BIO_get_mem_ptr(bio, &bptr);

		//bptr->data is not NULL terminated - add null character
		buf = (char *)malloc((bptr->length + 1) * sizeof(char));
		memcpy(buf, bptr->data, bptr->length);
		buf[bptr->length] = '\0';

		cout << "ext:" << buf << endl;
		BIO_free_all(bio);
		free(buf);
		
		*/
	}

	int entriesNum = sk_X509_NAME_ENTRY_num(subject->entries);
	//char *p=X509_NAME_oneline(issuer, NULL, 0);
	for (int i = 0; i < entriesNum; i++)
	{
		X509_NAME_ENTRY *name_entry = sk_X509_NAME_ENTRY_value(subject->entries, i);
		int Nid = OBJ_obj2nid(name_entry->object);
		switch (Nid)
		{
		case NID_countryName://国家
		{
			//cout << "C=";
			string ANSI;
			char msginfo[512] = {0};
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI <<"\t"<< "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["C"] = ANSI;
		}
			break;
		case NID_stateOrProvinceName://省
		{
			//cout<< "ST=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
				
			value["ST"] = ANSI;
		}
			break;
		case NID_localityName://地区
		{
			//cout << "L=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["L"] = ANSI;
		}
			break;
		case NID_organizationName://组织
		{
			//cout << "O=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["O"] = ANSI;
		}
		
			break;
		case NID_organizationalUnitName://单位
		{
		//	cout << "OU=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["OU"] = ANSI;
		}

			break;
		case NID_commonName://通用名
		{
			//cout << "CN=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["CN"] = ANSI;
		}
			break;
		case NID_pkcs9_emailAddress:   //Mail
		{
			//cout << "E=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["E"] = ANSI;
		}
			break;
		case NID_name:
		{
				//cout << "NID=";
				string ANSI;
				char msginfo[512] = { 0 };
				memcpy(msginfo, name_entry->value->data, name_entry->value->length);
				msginfo[name_entry->value->length] = '\0';
				if (V_ASN1_UTF8STRING == name_entry->value->type)
				{
					ANSI = msginfo;
					//ConverUtf8toANSI(ANSI, msginfo);
					//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
				}
				else
				{
					ANSI = msginfo;
					//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
				}
				value["NID"] = ANSI;
		}
		break;
		case NID_title:
		{
			//cout << "title=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			msginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
			ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["title"] = ANSI;
		}
		break;
		default:
		{
			//cout << "default=";
			string ANSI;
			char msginfo[512] = { 0 };
			memcpy(msginfo, name_entry->value->data, name_entry->value->length);
			m
sginfo[name_entry->value->length] = '\0';
			if (V_ASN1_UTF8STRING == name_entry->value->type)
			{
				ANSI = msginfo;
				//ConverUtf8toANSI(ANSI, msginfo);
				//cout << ANSI << "\t" << "type:" << name_entry->value->type << endl;
			}
			else
			{
				ANSI = msginfo;
				//cout << msginfo << "\t" << "type:" << name_entry->value->type << endl;
			}
			value["default"] = ANSI;
		}
			break;
		}
	}

	ASN1_TIME* begT=X509_get_notBefore(usrCert);
	ASN1_TIME* EndT = X509_get_notAfter(usrCert);
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

	{
		struct tm *local_time = NULL;
		time_t t1 = ASN1_GetTimeT(begT);
		//time_t t1 = ASN1_GetTimeT(EndT);
		
		char str_time[100];
		char szTime[100];
		struct tm tm1;  
		 #ifdef WIN32  
		
			tm1 = *localtime(&t1);  
		#else  
			localtime_r(&time1, &tm1 );  
		#endif  
		 sprintf( szTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",  
			   tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,  
				 tm1.tm_hour, tm1.tm_min,tm1.tm_sec);  
		//cout<<"begin time:"<<szTime<<endl;

		local_time = localtime(&t1);
		strftime(str_time, sizeof(str_time), "%Y-%m-%d:%H:%M:%S", local_time);

		cout<<"begin time:"<<str_time<<endl;
	}

	value["BeginTime"] = begTBuff;
	value["EndTime"] = EndTBuff;

	X509_free(usrCert);
	return  0;;
}

int readAllFileData(const char *fileName, char * result,int *pResultLen)
{
  int ret = 0;
  size_t nRead = 0;
  struct stat fileInfo={0};
  FILE *filePointer = 0;

  if((fileName==NULL) || (pResultLen == 0))
  {
      ret = -1;
      goto err;
  }


  if(!(filePointer=fopen(fileName,"rb ")))
  {
      ret = -2;
      goto err;
  }

  stat(fileName,&fileInfo);
  if(fileInfo.st_size > (*pResultLen)){
      ret = -3;
      goto err;
  }

  nRead = fread(result,1,fileInfo.st_size,filePointer);
  if(nRead != fileInfo.st_size){
      ret = -4;
      goto err;
  }

  ret = 0;
err: 
  if(pResultLen) {
    *pResultLen = fileInfo.st_size;
  }
  if(filePointer) fclose(filePointer);
  
  return ret;
}



int main(){
	std::map<string,string> m;
	char cert[4096];
	int certLen=sizeof(cert);
	readAllFileData("root.cer",cert,&certLen);

	GetX509Items((unsigned char*)cert,certLen,m);

	for (map<string, string>::iterator i=m.begin(); i!=m.end(); i++)
    {
		printf("%s:%s\n",i->first.c_str(),i->second.c_str());
    }
	return 1;
}
























