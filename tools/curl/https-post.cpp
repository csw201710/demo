#include "curl/curl.h"


static size_t _recv_data(void *contents, size_t sz, size_t nmemb, void *ctx)
{
	size_t realsize = 0;
	realsize = sz * nmemb;
	std::string *buf = (std::string*)ctx;
	std::string tmp((const char*)contents, realsize);
	(*buf) += tmp;
	return realsize;
}

int sendHttp(){
	int ret = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	string outStr;
	
	const char* url = "https://127.0.0.1:8082/ou/client/getLatestXMLData.json";
	struct curl_slist *headers = NULL;
	char *header_data = 0;
	std::string authCode = "";
	std::string current_time = "";
	std::string recvData;

	CURL* curl_handle = curl_easy_init();

	// add header
	asprintf(&header_data, "Version: %s","test");
	headers = curl_slist_append(headers, header_data);
	if(header_data) {free(header_data);header_data = 0;}

	
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
	
	outStr = "token=1";
	
	//set url
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	
	//add post data
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, outStr.c_str());
	
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, _recv_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &recvData);

	r = curl_easy_perform(curl_handle);


	if (r != CURLE_OK){
		ERROR("%s", curl_easy_strerror(r));
		ret = -1;goto err;

	}
	INFO("[return] %s", recvData.c_str());
	ret = 0;
err:
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl_handle);
	return ret;
}



