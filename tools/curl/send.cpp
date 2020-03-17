

static size_t _recv_data(void *contents, size_t sz, size_t nmemb, void *ctx)
{
	size_t realsize = 0;
	realsize = sz * nmemb;
	std::string *buf = (std::string*)ctx;
	std::string tmp((const char*)contents, realsize);
	(*buf) += tmp;
	return realsize;
}

int sendHttp(const char* url,const std::string &postData, std::string &result){
	int ret = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	//struct curl_slist *headers = NULL;
	char *header_data = 0;
	std::string authCode = "";
	std::string current_time = "";
	std::string recvData;

	CURL* curl_handle = curl_easy_init();

	// add header
	//asprintf(&header_data, "Version: %s","test");
	//headers = curl_slist_append(headers, header_data);
	//if(header_data) {free(header_data);header_data = 0;}
	//curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

	//set url
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	//add post data
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData.c_str());

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, _recv_data);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &recvData);

	r = curl_easy_perform(curl_handle);


	if (r != CURLE_OK){
		ERROR("%s", curl_easy_strerror(r));
		ret = -1;goto err;

	}
	INFO("[return] %s", recvData.c_str());
	ret = 0;
    result = recvData;
err:
	//curl_slist_free_all(headers);
	curl_easy_cleanup(curl_handle);
	return ret;
}




