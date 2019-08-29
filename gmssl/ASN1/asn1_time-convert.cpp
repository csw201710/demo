#include <time.h>


static time_t ASN1_GetTimeT(ASN1_TIME* time)
{
    struct tm t;
    const char* str = (const char*) time->data;
    size_t i = 0;

    memset(&t, 0, sizeof(t));

    if (time->type == V_ASN1_UTCTIME) {/* two digit year */
        t.tm_year = (str[i++] - '0') * 10;
        t.tm_year += (str[i++] - '0');
        if (t.tm_year < 70)
            t.tm_year += 100;
    } else if (time->type == V_ASN1_GENERALIZEDTIME) {/* four digit year */
        t.tm_year = (str[i++] - '0') * 1000;
        t.tm_year+= (str[i++] - '0') * 100;
        t.tm_year+= (str[i++] - '0') * 10;
        t.tm_year+= (str[i++] - '0');
        t.tm_year -= 1900;
    }
    t.tm_mon  = (str[i++] - '0') * 10;
    t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
    t.tm_mday = (str[i++] - '0') * 10;
    t.tm_mday+= (str[i++] - '0');
    t.tm_hour = (str[i++] - '0') * 10;
    t.tm_hour+= (str[i++] - '0');
    t.tm_min  = (str[i++] - '0') * 10;
    t.tm_min += (str[i++] - '0');
    t.tm_sec  = (str[i++] - '0') * 10;
    t.tm_sec += (str[i++] - '0');

    /* Note: we did not adjust the time based on time zone information */
    return mktime(&t);
}



void show()
	{
		struct tm *local_time = NULL;
		time_t t1 = ASN1_GetTimeT(begT);
		//time_t t1 = ASN1_GetTimeT(EndT);
		
		char str_time[100];
		char szTime[100];
		struct tm tm1;  
		 #ifdef WIN32  
		
			tm1 = *localtime(&t1);  
		#else  
			localtime_r(&time1, &tm1 );  
		#endif  
		 sprintf( szTime, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",  
			   tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,  
				 tm1.tm_hour, tm1.tm_min,tm1.tm_sec);  
		//cout<<"begin time:"<<szTime<<endl;

		local_time = localtime(&t1);
		strftime(str_time, sizeof(str_time), "%Y-%m-%d:%H:%M:%S", local_time);

		cout<<"begin time:"<<str_time<<endl;
	}















