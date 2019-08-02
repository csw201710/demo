#include <stdio.h>

#include "curl/curl.h"

#include <stdlib.h>

/* 从http头部获取文件size*/
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *)stream) = len;

	return size * nmemb;
}


/* 保存下载文件 */
size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE*)stream);
}
/*读取上传文件 */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE *)stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}



void showvVersion(){
	printf("curl_version : %s\n", curl_version());
	curl_version_info_data * data = curl_version_info(CURLVERSION_NOW);
}

int demo001(int argc, char *argv[])
{
	CURL *curl;             //定义CURL类型的指针
	CURLcode res;           //定义CURLcode类型的变量，保存返回状态码

	curl = curl_easy_init();        //初始化一个CURL类型的指针
	if (curl != NULL)
	{
		//设置curl选项. 其中CURLOPT_URL是让用户指定url. argv[1]中存放的命令行传进来的网址
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		//调用curl_easy_perform 执行我们的设置.并进行相关的操作. 在这里只在屏幕上显示出来.
		res = curl_easy_perform(curl);
		//清除curl操作.
		curl_easy_cleanup(curl);
	}
	return 0;
}


// 下载 文件函数
int download(CURL *curlhandle, const char * remotepath, const char * localpath,
	long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	int c;
	int use_resume = 0;
#ifndef _WIN32
	struct stat file_info;

	/* 得到本地文件大小 */
	//if(access(localpath,F_OK) ==0)
	if (stat(localpath, &file_info) == 0)
	{
		local_file_len = file_info.st_size;
		use_resume = 1;
	}
#else
	HANDLE handle = CreateFile(localpath, GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		int tmp = 0;
		tmp = GetFileSize(handle, NULL);
		CloseHandle(handle);
		if (tmp != 0)
		{
			local_file_len += (UINT64)tmp * 4294967296ULL;
		}
	}

#endif
	//采用追加方式打开文件，便于实现文件断点续传工作
	f = fopen(localpath, "ab+");
	if (f == NULL) {
		perror("fopen failed!");
		return 0;
	}

	//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);

	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒
	//设置http 头部处理函数
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	// 设置文件续传的位置给libcurl
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume ? local_file_len : 0);

	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);


	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	r = curl_easy_perform(curlhandle);

	fclose(f);

	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}


// 下载 文件函数
int upload(CURL *curlhandle, const char * remotepath, const char * localpath,
	long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1;
	long filesize = 0;
	CURLcode r = CURLE_GOT_NOTHING;
	int c;
	int use_resume = 0;
#ifndef _WIN32
	struct stat file_info;

	/* 得到本地文件大小 */
	//if(access(localpath,F_OK) ==0)
	if (stat(localpath, &file_info) == 0)
	{
		local_file_len = file_info.st_size;
		use_resume = 1;
	}
#else
	HANDLE handle = CreateFile(localpath, GENERIC_READ,
		FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		int tmp = 0;
		tmp = GetFileSize(handle, NULL);
		CloseHandle(handle);
		if (tmp != 0)
		{
			local_file_len += (UINT64)tmp * 4294967296ULL;
		}
	}

#endif
	//采用追加方式打开文件，便于实现文件断点续传工作
	f = fopen(localpath, "ab+");
	if (f == NULL) {
		perror("fopen failed!");
		return 0;
	}

	curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);

	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // 设置连接超时，单位秒


	curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);

	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
	r = curl_easy_perform(curlhandle);

	fclose(f);

	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}