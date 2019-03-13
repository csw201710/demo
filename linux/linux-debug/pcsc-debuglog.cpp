#include <syslog.h>
#include <stdarg.h>
#include<sys/time.h>
#define DEBUG_BUF_SIZE 2048

#define USE_DEBUGLOG_TIME_PREFIX

//DEBUG_TYPE
enum {
	DEBUG_TYPE_NO_DEBUG = 0, // no use log
	DEBUG_TYPE_SYSLOG_DEBUG,// use syslog log
	DEBUG_TYPE_STDOUT_DEBUG,
	DEBUG_TYPE_STDOUT_COLOR_DEBUG
};

//debug level
enum {
	DEBUG_LOG_DEBUG = 0,
	DEBUG_LOG_INFO,
	DEBUG_LOG_ERROR,   //only show DEBUG_LOG_ERROR DEBUG_LOG_CRITICAL
	DEBUG_LOG_CRITICAL
};
static char LogMsgType = DEBUG_TYPE_STDOUT_COLOR_DEBUG; //默认log类型
static char LogLevel = DEBUG_LOG_DEBUG; // 默认log level 
static void log_line(const int priority, const char *DebugBuffer);
void log_msg(const int priority, const char *fmt, ...)
{
	char DebugBuffer[DEBUG_BUF_SIZE];
	va_list argptr;

	if ((priority < LogLevel) /* log priority lower than threshold? */
		|| (DEBUG_TYPE_NO_DEBUG == LogMsgType))
		return;

	va_start(argptr, fmt);
	vsnprintf(DebugBuffer, sizeof DebugBuffer, fmt, argptr);
	va_end(argptr);

	log_line(priority, DebugBuffer);
} /* log_msg */

static void log_line(const int priority, const char *DebugBuffer)
{
	if (DEBUG_TYPE_SYSLOG_DEBUG == LogMsgType)
		syslog(LOG_INFO, "%s", DebugBuffer);
	else
	{
#ifdef USE_DEBUGLOG_TIME_PREFIX
		static struct timeval last_time = { 0, 0 };
		struct timeval new_time = { 0, 0 };
		struct timeval tmp;
		int delta;

		gettimeofday(&new_time, NULL);
		if (0 == last_time.tv_sec)
			last_time = new_time;

		tmp.tv_sec = new_time.tv_sec - last_time.tv_sec;
		tmp.tv_usec = new_time.tv_usec - last_time.tv_usec;
		if (tmp.tv_usec < 0)
		{
			tmp.tv_sec--;
			tmp.tv_usec += 1000000;
		}
		if (tmp.tv_sec < 100)
			delta = tmp.tv_sec * 1000000 + tmp.tv_usec;
		else
			delta = 99999999;

		last_time = new_time;
#else
		time_t now;
		struct tm* timenow;
		char szTime[128] = {0};
		time(&now);
		timenow = localtime(&now);
		sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%02d] ",
					   timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
					   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
#endif
		
		if (LogMsgType == DEBUG_TYPE_STDOUT_COLOR_DEBUG)
		{
			const char *color_pfx = "", *color_sfx = "\33[0m";
			const char *time_pfx = "\33[36m", *time_sfx = color_sfx;

			switch (priority)
			{
				case DEBUG_LOG_CRITICAL:
					color_pfx = "\33[01;31m"; /* bright + Red */
					break;

				case DEBUG_LOG_ERROR:
					color_pfx = "\33[35m"; /* Magenta */
					break;

				case DEBUG_LOG_INFO:
					color_pfx = "\33[34m"; /* Blue */
					break;

				case DEBUG_LOG_DEBUG:
					color_pfx = ""; /* normal (black) */
					color_sfx = "";
					break;
			}
#ifdef USE_DEBUGLOG_TIME_PREFIX
			printf("%s%.8d%s %s%s%s\n", time_pfx, delta, time_sfx,
				color_pfx, DebugBuffer, color_sfx);
#else
			printf("%s%s%s %s%s%s\n", time_pfx, szTime, time_sfx,
				color_pfx, DebugBuffer, color_sfx);
#endif
		}
		else
		{
#ifdef USE_DEBUGLOG_TIME_PREFIX
			printf("%.8d %s\n", delta, DebugBuffer);
#else
			printf("%s %s\n", szTime, DebugBuffer);
#endif
		}
		fflush(stdout);
	}
} /* log_msg */





void testLog(){
	log_msg(DEBUG_LOG_DEBUG,"PCSC_LOG_DEBUG show");
	log_msg(DEBUG_LOG_INFO,"PCSC_LOG_INFO show");
	log_msg(DEBUG_LOG_ERROR,"PCSC_LOG_ERROR show");
	log_msg(DEBUG_LOG_CRITICAL,"PCSC_LOG_CRITICAL show");
}




