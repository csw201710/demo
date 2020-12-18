#include "Log.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>

//#define _DEBUG
#define MAX_LOG_SIZE 32*1024*1024


static int _isquiet = 1;
static int _iscolor_log = 1;



static int check(){
  if(access(LOG_PATH, F_OK) == 0){
  	return 1;
  }
  return 0 ;
}

void LogString(unsigned long logType, const char *file, int line,const char* fmt, ...)
{
	//参数解析
	char *LogInfo = 0;
	char LogFile[256];
	char FileInfo[256];
	char Level[16];
	time_t now;
	struct tm* timenow;
	struct stat st;

	static int isLog = check();

	char szTime[128] = {0};
	int nLen = 0;

	va_list arg;

#if 1
	//设置日志路径
	strcpy(LogFile, LOG_PATH);
	switch(logType) {
	case MY_LOG_INFO:
		strcpy(Level,"[info   ] ");
		break;
	case MY_LOG_WARNING:
		strcpy(Level,"[warning] ");
		break;
	case MY_LOG_ERROR:
		strcpy(Level,"[error  ] ");
		break;
	default:
		break;
	}
#else
  Level[0] = '\0';
#endif


	va_start(arg, fmt);
	vasprintf(&LogInfo, fmt, arg);
	va_end(arg);
 if(LogInfo == 0) {
  fprintf(stderr,"LogString failed!\n");
  return ;
 }


	time(&now);
	timenow = localtime(&now);
	nLen = sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%02d] ",
				   timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
				   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);

#if 1
	sprintf(FileInfo,"[%s:%d]",file,line);
#else
  FileInfo[0] = '\0';
#endif
	switch(logType){
	case MY_LOG_INFO:
		fprintf(stderr, "%s%s%s%s\n", szTime,FileInfo,Level,LogInfo);
		break;
	case MY_LOG_WARNING:
		fprintf(stderr, "\33[34m%s%s%s%s\33[0m\n", szTime,FileInfo,Level,LogInfo);
		break;
	case MY_LOG_ERROR:
		fprintf(stderr, "\33[01;31m%s%s%s%s\33[0m\n", szTime,FileInfo,Level,LogInfo);
		break;
	default:
		break;
	}


	if(!isLog) {
	  free(LogInfo);
	  return ;
	 }

	//回滚日志
	if (stat(LogFile, &st)==0 && st.st_size>MAX_LOG_SIZE)
	{
		unlink(LogFile);
	}

	FILE* f = NULL;
	f=fopen(LogFile, "a+");
	if (f)
	{
		fwrite(szTime, 1, nLen, f);
		fwrite(FileInfo, 1, strlen(FileInfo), f);
		fwrite(Level, 1, strlen(Level), f);
		fwrite(LogInfo, 1, strlen(LogInfo), f);
		fwrite("\n", 1, 1, f);
		fclose(f);
	}
 free(LogInfo);

}

void LogData(unsigned long logType,const char* prompt, unsigned char* data, unsigned long len)
{
	FILE *pFile;
	unsigned long i;
	char LogFile[256];
	char szBuf[256] = {0};
	char Level[16];
	time_t now;
	struct tm* timenow;
	int nLen = 0;
	struct stat st;

	static int isLog = check();

	 if(isLog == 0){
			return;
		}
	//设置日志路径
	strcpy(LogFile, LOG_PATH);
	switch(logType) {
	case MY_LOG_INFO:

		strcpy(Level,"[info   ] ");
		break;
	case MY_LOG_WARNING:
		strcpy(Level,"[warning] ");
		break;
	case MY_LOG_ERROR:
		strcpy(Level,"[error  ] ");
		break;
	default:
		break;
	}

	time(&now);
	timenow = localtime(&now);
	nLen = snprintf(szBuf, sizeof(szBuf) -1,"[%04d-%02d-%02d %02d:%02d:%02d] %s %s\n",
		timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
		timenow->tm_hour, timenow->tm_min, timenow->tm_sec,Level, prompt);

	//回滚日志
	if (stat(LogFile, &st)==0 && st.st_size>MAX_LOG_SIZE)
	{
		unlink(LogFile);
	}

	if((pFile=fopen(LogFile,"a+"))!=NULL)
	{
		//打印提示语
		fprintf(pFile, szBuf);
		//打印输入信息
		for(i=0;i<len;i++)
		{
			if(i!=0 && i%32 == 0)
				fprintf(pFile,"\n");

			if(*(data+i)<=0xf){
				fprintf(pFile,"0%x ",*(data+i));
			}
			else{
				fprintf(pFile,"%x ",*(data+i));
			}
		}
		fprintf(pFile,"\n");
		//关闭文件
		fclose(pFile);
	}
}



/*

static void pusb_log_syslog(int level, const char *format, va_list ap)
{
	openlog(PROGRAM_NAME, LOG_PID, LOG_AUTH);
	vsyslog(level, format, ap);
	closelog();
}



static void pusb_log_output(int level, const char *format, va_list ap)
{
	if (!isatty(fileno(stdin)))
		return ;
	if (_isquiet != 0)
	{
		if (_iscolor_log != 0)
		{
			if (level == LOG_ERR)
				fprintf(stderr, "\033[01;31m*\033[00m ");
			else //if (level == LOG_NOTICE)
				fprintf(stderr, "\033[01;32m*\033[00m ");
		}
		else
			fprintf(stderr, "* ");
		vfprintf(stderr, format, ap);
	}
}


void __log_debug(const char *file, int line, const char *fmt, ...)
{
	va_list	ap;
	fprintf(stderr, "[%s:%03d] ", file, line);
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	va_start(ap, fmt);
	pusb_log_syslog(LOG_DEBUG, fmt, ap);
	va_end(ap);
}

void log_error(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	pusb_log_syslog(LOG_ERR, fmt , ap);
	va_end(ap);

	va_start(ap, fmt);
	pusb_log_output(LOG_ERR, fmt , ap);
	va_end(ap);
}

void log_info(const char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	pusb_log_syslog(LOG_NOTICE, fmt, ap);
	va_end(ap);

	va_start(ap, fmt);
	pusb_log_output(LOG_NOTICE, fmt, ap);
	va_end(ap);
}




void print_arr(const char *string, unsigned char*data, int size)
{
	int i;

	printf("---------------------------------------------\n");
	printf("%s len=%d\n", string,size);
	for(i=0; i<size; i++)
	{
		printf("%02x ", data[i]);
		if((i%16) == 15)
			printf("\n");
	}
	if(size%16)
		printf("\n");
	printf("---------------------------------------------\n");
}
*/
