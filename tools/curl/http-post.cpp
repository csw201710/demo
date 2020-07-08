#include "Internet.h"
#include "curl\curl.h"

#include "LOGC.h"

#pragma comment(lib, "libcurl_a.lib")

static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string *str = (std::string*)stream;
	(*str).append((char*)ptr, size*nmemb);
	return size * nmemb;
}

static std::string get_cookies(CURL *curl)
{
	CURLcode res;
	std::string result;
	struct curl_slist *cookies;
	struct curl_slist *nc;
	int i;

	res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
	if (res != CURLE_OK) {
		fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n",
			curl_easy_strerror(res));
		exit(1);
	}
	nc = cookies;
	i = 1;
	while (nc) {
		result += nc->data;
		result += "; ";
		nc = nc->next;
		i++;
	}
	curl_slist_free_all(cookies);

	return result;
}


int curl_post_req(const char* url, const std::string &postParams, std::string &response) {
	std::string cookie;
	return curl_post_req(url, postParams, cookie, response);
}




std::string param_escape(const char* data) {
	return param_escape(data, strlen(data));
}

std::string param_escape(const char* data, int dataLen) {
	std::string res;
	if (dataLen > 0) {
		char *d;
		d = curl_easy_escape(0, data, dataLen);
		res = d;
		curl_free(d);
	}
	return res;
}


int curl_post_req(const char* url, const std::string &postParams, std::string& cookie, std::string &response)
{
	int ret = 0;
	CURLcode res;
	struct curl_slist* headers = NULL;
	CURL *curl = 0;

	curl = curl_easy_init();
	if (curl == 0) {
		LOG::ERRORLOG("curl_easy_init failed!");
		ret = -1;goto err;
	}
	
	
	//sprintf_s(tmp_str, "Content-Length: %s", jsonout.c_str());
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


	curl_easy_setopt(curl, CURLOPT_URL, url); // url

	// set params
	curl_easy_setopt(curl, CURLOPT_POST, 1); // post req

	// 设置要POST的JSON数据
	if (postParams.length() > 0) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());
	}
		

	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
	//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	//curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

	//curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(curl, CURLOPT_HEADER, 1);
	//curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	// start req
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		LOG::ERRORLOG("curl perform failed: %s\n", curl_easy_strerror(res));
		ret = -1;goto err;
	}
	cookie = get_cookies(curl);

	ret = 0;
err:
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	return ret;
}


