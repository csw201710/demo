#include <stdio.h>

#include "curl/curl.h"

#include <stdlib.h>
#include <string>
#include <regex>
#include <iostream>


#include <windows.h>

std::string GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	//size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	std::string* data = (std::string* ) stream;

	size_t count = size * nmemb;
	data->append((const char *)ptr, count);
	return size * nmemb;
}

/*
//setlocale(LC_ALL, "UTF-8");
SetConsoleOutputCP(65001);
*/
int getPublibIP(std::string &ip, std::string& address){
	CURL *curl_handle;
	int ret;
	std::string data;
	struct curl_slist *chunk = NULL;
	std::string domains[3] = { "api.ipify.org", "bot.whatismyipaddress.com", "icanhazip.com" };

	curl_global_init(CURL_GLOBAL_ALL);
	/* init the curl session */
	curl_handle = curl_easy_init();
	


	

	/* Remove a header curl would otherwise add by itself */
	//chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:73.0) Gecko/20100101 Firefox/73.0");
	//chunk = curl_slist_append(chunk, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
	//chunk = curl_slist_append(chunk, "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
	//chunk = curl_slist_append(chunk, "Accept-Encoding: gzip, deflate, br");
	//chunk = curl_slist_append(chunk, "Cookie: ASPSESSIONIDASRCBADC=PHEBENJBBGNCHBCILOOGHMND");
	//curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, chunk);
	
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://2020.ip138.com/");//http://www.ip138.com/  "https://2020.ip138.com/"
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	/*

	CURL_SSLVERSION_SSLv2,
		CURL_SSLVERSION_SSLv3,
		CURL_SSLVERSION_TLSv1_0,
		CURL_SSLVERSION_TLSv1_1,
		CURL_SSLVERSION_TLSv1_2,
		CURL_SSLVERSION_TLSv1_3,

		CURL_SSLVERSION_LAST // never use, keep last 
	*/
	curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
	/* Switch on full protocol/debug output while testing */
	//curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &data);

	/* get it! */
	CURLcode r = curl_easy_perform(curl_handle);
	if (r != CURLE_OK){
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		ret = -1;  goto err;
	}
	


	{
		std::regex reg("\\[(.*?)\\] 来自：(.*?)\r\n");
		std::smatch matchResult;
		std::string gbk = Utf8ToGbk(data.c_str());
		//正则匹配
		//fprintf(stdout, "%s\n", gbk.c_str());
		
		std::string s("abcdefsubject");std::regex e(".*?sub(.*)");
#if 0
		if (std::regex_match(gbk.cbegin(), gbk.cend(), matchResult, reg))
		{
			std::cout << "Match: " << matchResult.size() << " \n";
			//打印子表达式结果
			for (size_t i = 1; i < matchResult.size(); ++i)
			{
				std::cout << matchResult[i] << " ";
			}
		}
		else{
			std::cout << "Not Match!\n";
		}
#else
		const char* tmp = gbk.c_str();
		if (std::regex_search(gbk, matchResult, reg)) {
			/*
			for (size_t i = 1; i < matchResult.size(); ++i)
			{
				std::cout << matchResult[i] << "\n";
			}
			gbk = matchResult.suffix().str();
			*/
			if (matchResult.size() >= 2) ip = matchResult[1];
			if (matchResult.size() >= 3) address = matchResult[2];
			
		}
		else{
			std::cout << "Not Match!\n";
			ret = -2; goto err;
		}
#endif
	
	}


	ret = 0;
err:
	/* cleanup curl stuff */
	curl_easy_cleanup(curl_handle);
	curl_slist_free_all(chunk);
	curl_global_cleanup();
	return ret;



}


void testShowPublicIP(){
	int ret;
	std::string s1, s2;
	//setlocale(LC_ALL, "UTF-8");
	//SetConsoleOutputCP(65001);
	//ip: [61.177.144.130] address:[江苏省无锡市 电信]
	ret = getPublibIP(s1, s2);
	if (ret == 0){
		printf("ip: [%s] address:[%s]\n", s1.c_str(), s2.c_str());
	}

}