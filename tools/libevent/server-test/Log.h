#ifndef __LOG_H_H__
#define __LOG_H_H__

#define		MY_LOG_INFO	0
#define   MY_LOG_WARNING 1
#define		MY_LOG_ERROR	2
#include <stdio.h>
#include <string.h>
#include <stdlib.h>






#define LOG_PATH "/tmp/abcdefghijk.log"


void LogString(unsigned long logType, const char *file, int line,const char* fmt, ...);
void LogData(unsigned long logType, char* prompt, unsigned char* data, unsigned long len);
void print_arr(const char *string, unsigned char*data, int size);

#ifdef _DEBUG
#define ERROR(...) \
	do{\
		LogString(MY_LOG_ERROR,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define INFO(...) \
	do{\
		LogString(MY_LOG_INFO,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define WARN(...) \
	do{\
		LogString(MY_LOG_WARNING,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define log_debug(s, ...) __log_debug(__FILE__, __LINE__, s, ##__VA_ARGS__);

#define DEBUG_ARR(...) print_arr(__VA_ARGS__);

#else

#define ERROR(...) \
	do{\
		LogString(MY_LOG_ERROR,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define WARN(...) \
	do{\
		LogString(MY_LOG_WARNING,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define INFO(...)



#define DEBUG_ARR(...)

#define log_debug(s, ...)
#endif


#define DebugLog(a,b,c, ...) \
    INFO(__VA_ARGS__);

#define WriteErrorLog(a,b,c, ...) \
    ERROR(__VA_ARGS__);


void __log_debug(const char *file, int line, const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_info(const char *fmt, ...);




#endif //__LOG_H_H__
