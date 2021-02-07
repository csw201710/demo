#ifndef __LOG_H_H__
#define __LOG_H_H__


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOG_NAMESPACE_XX  rsladfjalsdkfjasd

namespace LOG_NAMESPACE_XX{



#define		MY_LOG_INFO	0
#define   MY_LOG_WARNING 1
#define		MY_LOG_ERROR	2


#define LOG_PATH "/tmp/ib_svsserver_.log"


void InitLogFactory();
void writelogStringToFile(unsigned int logType, const char *file, int line,const char* fmt, ...);


#ifdef _DEBUG

#define ERROR(...) \
	do{\
		writelogStringToFile(MY_LOG_ERROR,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define INFO(...) \
	do{\
		writelogStringToFile(MY_LOG_INFO,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define WARN(...) \
	do{\
		writelogStringToFile(MY_LOG_WARNING,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#else

#define ERROR(...) \
	do{\
		writelogStringToFile(MY_LOG_ERROR,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define WARN(...) \
	do{\
		writelogStringToFile(MY_LOG_WARNING,__FILE__,__LINE__,__VA_ARGS__);\
	}while(0);

#define INFO(...)

#define DEBUG_ARR(...)

#endif

};
using namespace LOG_NAMESPACE_XX;

#endif //__LOG_H_H__
