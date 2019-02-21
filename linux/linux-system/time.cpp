/*
** SYSTEMTIME转time_t
struct tm结构和struct SYSTEMTIME结构的年和月的取值范围是不一样的:

tm.tm_mon = systemtime.wMonth - 1

tm.tm_year = systemtime.wYear - 1900
*/

time_t systime_to_timet(const SYSTEMTIME& st)
{
    struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
    return mktime(&gm);
}

/*

**time_t转SYSTEMTIME

*/

SYSTEMTIME Time_tToSystemTime(time_t t)

{

    tm temptm = *localtime(&t);

    SYSTEMTIME st = {1900 + temptm.tm_year, 

                                   1 + temptm.tm_mon, 

                                   temptm.tm_wday, 

                                   temptm.tm_mday, 

                                   temptm.tm_hour, 

                                   temptm.tm_min, 

                                   temptm.tm_sec, 

                                   0};

    return st;

}

