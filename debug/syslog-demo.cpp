#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>

#define log_debug(fmt, ...)      syslog(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)       syslog(LOG_INFO, fmt, ##__VA_ARGS__)
#define log_notice(fmt, ...)     syslog(LOG_NOTICE, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...)    syslog(LOG_WARNING, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...)      syslog(LOG_ERR, fmt, ##__VA_ARGS__)
#define log_crit(fmt, ...)       syslog(LOG_CRIT, fmt, ##__VA_ARGS__)
#define log_alert(fmt, ...)      syslog(LOG_ALERT, fmt, ##__VA_ARGS__)
#define log_emerge(fmt, ...)     syslog(LOG_EMERG, fmt, ##__VA_ARGS__)

int main(int argc, char **argv)
{
    openlog("syslog_test", LOG_PID, LOG_DAEMON);

    log_debug("debug-level message");
    log_info("informational message");
    log_notice("normal, but significant, condition");
    log_warning("warning conditions");
    log_error("error conditions");
    log_crit("critical conditions");
    log_alert("action must be taken immediately");
    log_emerge("system is unusable");

    closelog();

    return 0;
}

