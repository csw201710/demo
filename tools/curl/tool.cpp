//url 参数编码
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

