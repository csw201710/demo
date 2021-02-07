#include "Log.h"
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

#define MAX_LOG_SIZE 35*1024*1024


namespace LOG_NAMESPACE_XX{

static void _getCurtime(char szTime[256]){
    time_t now;
    struct tm* timenow;
    time(&now);
	timenow = localtime(&now);
	sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%02d] ",
				   timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
				   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
}

//static std::shared_ptr<spdlog::logger> file_log = spdlog::basic_logger_mt<spdlog::async_factory>("async_svs", LOG_PATH);
//with 5mb size max and 3 rotated files.
static std::shared_ptr<spdlog::logger> file_log = spdlog::rotating_logger_mt("rotate_svs", LOG_PATH, MAX_LOG_SIZE, 3);
//
//static std::shared_ptr<spdlog::logger> file_log = spdlog::daily_logger_mt("daily_logger", LOG_PATH, 00, 00,false, 5);
static std::shared_ptr<spdlog::logger> console = spdlog::stdout_color_mt("console");


//
void InitLogFactory(){
    spdlog::level::level_enum level = spdlog::level::debug;

    console->set_level(level);
    console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");// [%n] logger name

    file_log->flush_on(spdlog::level::info);//err
    file_log->set_level(level);
    file_log->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
}


void writelogStringToFile(unsigned int logType, const char *file, int line,const char* fmt, ...) {
    //参数解析
	char *LogInfo = 0;
	va_list arg;

	va_start(arg, fmt);
	vasprintf(&LogInfo, fmt, arg);
	va_end(arg);
    if(LogInfo == 0) {
      fprintf(stderr,"LogString failed!\n");
      return ;
    }

    switch(logType) {
 	case MY_LOG_INFO:
        console->info(LogInfo);
        file_log->info(LogInfo);
 		break;
 	case MY_LOG_WARNING:
        console->warn(LogInfo);
        file_log->warn(LogInfo);
 		break;
 	case MY_LOG_ERROR:
        console->error(LogInfo);
        file_log->error(LogInfo);
       break;
 	default:
 		break;
 	}

    free(LogInfo);



}

}


