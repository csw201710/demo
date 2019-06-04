#ifndef _WIN32

#include <errno.h>
#include <iconv.h>
#include <string.h>
#else
#include <windows.h>
#endif

bool gbk_to_utf8 (char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{
  /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
   *           IGNORE ：遇到无法转换字符跳过*/
  const char encTo[] = "UTF-8//IGNORE";

/* 源编码 */
  const char encFrom[] = "GBK";

  /* 获得转换句柄
   *@param encTo 目标编码方式
   *@param encFrom 源编码方式
   *
   * */
  iconv_t cd = iconv_open (encTo, encFrom);
  if (cd == (iconv_t)-1)
  {
     perror ("iconv_open");
  }

  /* 需要转换的字符串 */
  //printf("inbuf=%s\n", inbuf);

  /* 打印需要转换的字符串的长度 */
  //printf("inlen=%d\n", *inlen);


  /* 由于iconv()函数会修改指针，所以要保存源指针 */
  char *tmpin = inbuf;
  char *tmpout = outbuf;
  size_t insize = *inlen;
  size_t outsize = *outlen;

  /* 进行转换
   *@param cd iconv_open()产生的句柄
   *@param srcstart 需要转换的字符串
   *@param inlen 存放还有多少字符没有转换
   *@param tempoutbuf 存放转换后的字符串
   *@param outlen 存放转换后,tempoutbuf剩余的空间
   *
   * */
  size_t ret = iconv (cd, &tmpin, inlen, &tmpout, outlen);
  if (ret == -1)
  {
     perror ("iconv");
  }

  /* 存放转换后的字符串 */
  //printf("outbuf=%s\n", outbuf);

  //存放转换后outbuf剩余的空间
  //printf("outlen=%d\n", *outlen);
/*
  int i = 0;

  for (i=0; i<(outsize- (*outlen)); i++)
  {
     //printf("%2c", outbuf[i]);
     //printf("%x\n", outbuf[i]);
  }
*/
  /* 关闭句柄 */
  iconv_close (cd);

  return 0;
}


bool utf8_to_gbk (char *inbuf, size_t *inlen, char *outbuf, size_t *outlen)
{

  /* 目的编码, TRANSLIT：遇到无法转换的字符就找相近字符替换
   *           IGNORE ：遇到无法转换字符跳过*/
  const char *encTo = "GBK//IGNORE";
  /* 源编码 */
  const char *encFrom = "UTF-8";

  /* 获得转换句柄
   *@param encTo 目标编码方式
   *@param encFrom 源编码方式
   *
   * */
  iconv_t cd = iconv_open (encTo, encFrom);
  if (cd == (iconv_t)-1)
  {
      perror ("iconv_open");
  }

  /* 需要转换的字符串 */
  //printf("inbuf=%s\n", inbuf);

  /* 打印需要转换的字符串的长度 */
 // printf("inlen=%d\n", *inlen);

  /* 由于iconv()函数会修改指针，所以要保存源指针 */
  char *tmpin = inbuf;
  char *tmpout = outbuf;
  size_t insize = *inlen;
  size_t outsize = *outlen;

  /* 进行转换
   *@param cd iconv_open()产生的句柄
   *@param srcstart 需要转换的字符串
   *@param inlen 存放还有多少字符没有转换
   *@param tempoutbuf 存放转换后的字符串
   *@param outlen 存放转换后,tempoutbuf剩余的空间
   *
   * */
  size_t ret = iconv (cd, &tmpin, inlen, &tmpout, outlen);
  if (ret == -1)
  {
     perror ("iconv");
  }

  /* 存放转换后的字符串 */
  //printf("outbuf=%s\n", outbuf);

  //存放转换后outbuf剩余的空间
  //printf("outlen=%d\n", *outlen);

  int i = 0;
/*
  for (i=0; i<(outsize- (*outlen)); i++)
  {
     //printf("%2c", outbuf[i]);
     printf("%x\n", outbuf[i]);
  }
*/
  /* 关闭句柄 */
  iconv_close (cd);

  return 0;
}


void ConverANSItoUtf8(std::string&amp, std::string strUtf8)
{
#if 0
	//CP_ACP：ANSI字符集 指定执行转换的字符集
	//dwFlags：一组位标记用以指出是否未转换成预作或宽字符
	//lpMultiByteStr：指向将被转换字符串的字符。
	//cchMultiByte：指定由参数lpMultiByteStr指向的字符串中字节的个数。如果lpMultiByteStr指定的字符串以空字符终止，可以设置为-1
	//lpWideCharStr：指向接收被转换字符串的缓冲区。
	//cchWideChar：指定由参数lpWideCharStr指向的缓冲区的宽字符个数。若此值为零，函数返回缓冲区所必需的宽字符数，
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strUtf8.c_str(), -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t [len + 1];
	memset(wszGBK, 0, len +1);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strUtf8.c_str(), -1, wszGBK, len);
	
	//CP_UTF8：使用UTF-8转换。
	//允许你进行额外的控制，它会影响使用了读音符号（比如重音）的字符
	//指定要转换为宽字节字符串的缓冲区
	//指定由参数lpWideCharStr指向的缓冲区的字符个数
	//指向接收被转换字符串的缓冲区
	//指定由参数lpMultiByteStr指向的缓冲区最大值
	//遇到一个不能转换的宽字符，函数便会使用pDefaultChar参数指向的字符
	//至少有一个字符不能转换为其多字节形式，函数就会把这个变量设为TRUE
	len = WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	//strUtf8 = szGBK; 
	amp = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
#else
	//TODO
	size_t  srclen = strUtf8.length();
	size_t len = srclen * 6;
	char * szGBK = new char [len];
	memset(szGBK, 0, len );
	gbk_to_utf8((char *)strUtf8.c_str(), &srclen,szGBK,&len);
	amp = szGBK;
	delete []szGBK;
#endif
}

void ConverUtf8toANSI(std::string&amp, std::string strUtf8)
{
#if 0
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, NULL, 0);
	wchar_t * wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	//strUtf8 = szGBK; 
	amp = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
#else 
	//TODO  ./WebSocketsPlus.cpp:101:		ConverUtf8toANSI(tmp, tmp);
	size_t  srclen = strUtf8.length();
	size_t len = srclen * 6;
	char * szGBK = new char [len];
	memset(szGBK, 0, len );
	utf8_to_gbk((char *)strUtf8.c_str(), &srclen,szGBK,&len);
	amp = szGBK;
	delete []szGBK;
#endif
}

