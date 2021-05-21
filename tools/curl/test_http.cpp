
#include <curl/curl.h>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <stdarg.h>

#define DEBUG(...) \
	do{\
	  printf("[DEBUG  ] ");\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0);

#define INFO(...) \
	do{\
	  printf("[INFO  ] ");\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0);
	
#define ERROR(...) \
	do{\
	  printf("[ERROR  ] ");\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0);


#define WARN(...) \
	do{\
	  printf("[WARN  ] ");\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0);
		
	
	

static std::string StrFormat(const char *pszFmt, ...)
{
  char *buf = 0;
	std::string str;
	va_list args;
	va_start(args, pszFmt);
	vasprintf(&buf, pszFmt, args);
	if(buf != 0){
	  str = buf;
	  free(buf);
	}else{
	  ERROR("vasprintf failed");
	}

	va_end(args);
	return str;
}


static std::string getResponseTime(){
    std::string ret;
    char buf[1024] = {0};
    time_t seconds = time(0);
    //pTmp_tm =  localtime(&seconds);
    //struct tm *pTmp_tm =  gmtime(&seconds);
    struct tm utc_tm;
    if (0 == gmtime_r(&seconds, &utc_tm)){
        ERROR("gmtime_r failed!");
        return ret;
    }
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%SZ+0800",&utc_tm);
    ret = buf;
    return ret;
}
	
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
		ERROR("Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
		return "";
	}
	nc = cookies;
	i = 1;
	while (nc) {
		result += nc->data;
		nc = nc->next;
		i++;
		break;
	}
	curl_slist_free_all(cookies);

	return result;
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



std::string param_unescape(const char* data, int dataLen) {
	std::string res;
	if (dataLen > 0) {
		char *d;
        int outLen = 0;
		d = curl_easy_unescape(0, data, dataLen, &outLen);
		res.append(d, outLen);
		curl_free(d);
	}
	return res;
}


std::string param_escape(const std::string& data) {
	return param_escape(data.data(), data.length());
}

std::string param_unescape(const std::string& data) {
	return param_unescape(data.data(), data.length());
}

static void vStringSplit(const  std::string& s, std::vector<std::string>& elems,const std::string delim="&")
{
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0){
        return;
    }

    while (pos < len)
    {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0)
        {
            elems.push_back(s.substr(pos, len - pos));
            break;
        }
        elems.push_back(s.substr(pos, find_pos - pos));
        pos = find_pos + delim_len;
    }
    return;
}

static void parseResult(const std::string result, std::map<std::string, std::string>& map){
    std::vector<std::string> keyValue;
    vStringSplit(result, keyValue);

    for(size_t i = 0; i< keyValue.size();i++)
    {
        std::vector<std::string> kv;
        std::string item = keyValue[i];

        vStringSplit(item, kv, "=");
        //DEBUG("item:[%s]", item.c_str());
        if(kv.size() == 2){
            map[kv[0]] = param_unescape(kv[1]);
        }else if(kv.size() == 1){
            map[kv[0]] = "";
        }else{
            ERROR("invalid item : [%s]", item.c_str());
        }
    }
}

static std::string buildRequest(std::map<std::string, std::string>& params) {
  std::string result;
  std::map<std::string, std::string>::iterator it;
  for( it = params.begin(); it != params.end(); ++it)
  {
    if(result.length() != 0){
      result += "&";
    }
    result += it->first;
    result += "=";
    result += param_escape(it->second);
  }
  return result;
}



int curl_post_req(const char* url, const char* content_type,const std::string &postParams, std::string& cookie, std::string &response)
{
	int ret = 0;
	CURLcode res;
	struct curl_slist* headers = NULL;
	CURL *curl = 0;
    std::string request_time = "Request-Time: ";

	curl = curl_easy_init();
	if (curl == 0) {
		ERROR("curl_easy_init failed!");
		ret = -1;goto err;
	}
    if(content_type != 0){
        headers = curl_slist_append(headers, content_type);
    	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    headers = curl_slist_append(headers, "Request-Version: v1");
    request_time += getResponseTime();
    headers = curl_slist_append(headers, request_time.c_str());



	curl_easy_setopt(curl, CURLOPT_URL, url);
	//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	if (postParams.length() > 0) {
        DEBUG("postParams : [%s]", postParams.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_COOKIELIST, cookie.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		ERROR("http perform failed[%s]: %s",url, curl_easy_strerror(res));
		ret = -1;goto err;
	}
    DEBUG("response : [%s]", response.c_str());
	cookie = get_cookies(curl);

	ret = 0;
err:
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	return ret;
}

int curl_post_req(const char* url, const char* content_type,const std::string &postParams, std::string& cookie, std::map<std::string, std::string>& response){
    int ret;
    std::string result;
    ret = curl_post_req(url, content_type, postParams, cookie, result);
    if(ret != 0){
        return ret;
    }
    parseResult(result, response);
    return 0;
}

int curl_post_req(const char* url, const std::string &postParams,  std::map<std::string, std::string>& response){
    int ret;
    std::string result;
    std::string cookie;
    ret = curl_post_req(url, 0, postParams, cookie, result);
    if(ret != 0){
        return ret;
    }
    parseResult(result, response);
    return 0;
}



int main(){
  char url[1024] = {0};
  const char* ip = "x.x.x.x";
  int port = 12345;
  int ret;
  std::string xxx;
    
  //SDF_OpenDevice
  {
    std::map<std::string, std::string> params;
    std::map<std::string, std::string> response;    
    
    sprintf(url,"http://%s:%d/xxxx/xxx",ip,port);
    std::string body = buildRequest(params);
    ret = curl_post_req(url, body, response);
    if(ret != 0){
      ERROR("curl_post_req failed");
      
    }else{
      xxx = response["xxx"];
      INFO("xxx: [%s]",xxx.c_str());  
    }
    
  }

  
  return 0;
}



