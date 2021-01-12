#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} ST;



#define EACH_MINUTE_SECOND (60)
#define EACH_HOUR_SECOND (60 * 60)
#define EACH_DAY_SEDOND  (24 * 60 * 60)


static time_t getYearSecond(int year){
  int m_leap [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  time_t total = 0;
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0){
    m_leap[1] = 29;
  }
  for(int i = 0;i<12;i++){
    total += (m_leap[i] * EACH_DAY_SEDOND);
  }
  return total;
}

static void _addHour(ST* st, int hour){
  int m_leap [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  time_t   totalSecond;
  if(!st){
    return;
  }
  if(st->month > 12 || st->month < 1){
    return;
  }
  if(st->year < 1970){
    return;
  }
  if(hour < 0){
    return;
  }
  if ((st->year % 4 == 0 && st->year % 100 != 0) || st->year % 400 == 0){
    m_leap[1] = 29;
  }
  //每秒
  totalSecond = st->second;
  //每分
  totalSecond += (st->minute * EACH_MINUTE_SECOND);
  //每小时
  totalSecond += (st->hour * EACH_HOUR_SECOND);
  //每天
  if(st->day - 1 > 0){
    totalSecond += ((st->day-1) * EACH_DAY_SEDOND);  
  }
  
  // 每月
  for( int i= 1; i <= st->month - 1;i++){
    totalSecond += (m_leap[i - 1] * EACH_DAY_SEDOND); 
  }
  // 每年
  for(int i=st->year -1; i >= 1970;i--){
    totalSecond += getYearSecond(i);
  }
  
  //加上增加的时间
  totalSecond += (hour * EACH_HOUR_SECOND);

  char buf[1024] = {0};
  struct tm *pTmp_tm =  gmtime(&totalSecond);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",pTmp_tm);
  printf("gmtime : [%s]\n",buf);
  
  struct tm *pTmp_tm2 =  localtime(&totalSecond);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",pTmp_tm2);
  printf("localtime : [%s]\n",buf);
}



static void _addHour(const char* tt, const char* format, int hour){
  struct tm tmp_tm = {0};
  struct tm *pTmp_tm;
  char buf[1024] = {0};
  //strptime("6 Dec 2001 12:33:45", "%d %b %Y %H:%M:%S", &tm);
  strptime(tt, format, &tmp_tm);
  time_t seconds = mktime(&tmp_tm);
  seconds += (hour * EACH_HOUR_SECOND);
  pTmp_tm =  localtime(&seconds);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",pTmp_tm);
  printf("buf : [%s]\n",buf);
}


static int formatGMTToLocal(const char* t, char* outBuf, int outBufLen){
  //Jan 11 01:51:35.490977 2021 GMT
  
  const char* p[5] = {0};
  char *buf;
  char tmp[1024] = {0};
    
  ST st = {0};

  char *pp[3] = {0};
  char *tmp2 = 0;
  char * buf2;
  
  
  char tmp3[1024] = {0};
  
  const char *eng_month[] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"
  };
  if(t == 0 || strlen(t) == 0) {
    return -1;
  }
  
  strncpy(tmp, t, sizeof(tmp) - 1);
  
  for (int i=0;i<5;i++){
    p[i]=strtok_r(i == 0 ? tmp: 0, " ",&buf);    
  }
  
  printf("p[0]:%s\n",p[0]);
  printf("p[1]:%s\n",p[1]);
  printf("p[2]:%s\n",p[2]);
  printf("p[3]:%s\n",p[3]);
  printf("p[4]:%s\n",p[4]);
  
  
  st.year = atoi(p[3] != 0 ? p[3] : "");
    
  for(int i=0; p[2] != 0 && i<strlen(p[2]);i++){
    if(p[2][i] == '.'){
      char *t = (char*)p[2];
      t[i] = '\0';
      break;
    }
  }
  for(int i=0;p[0] !=0 && i<sizeof(eng_month);i++){
    if(strcmp(eng_month[i],p[0]) == 0){
      //char *t = (char*)p[0]; //sprintf(t, "%02d", (i+1));
      st.month = i+1;
      break;
    }
  }
  
  
  
  st.day = atoi(p[1] != 0 ? p[1] : "");
  
  tmp2 = (char*)p[2];
  for( int i=0;i< 3;i++) {
    pp[i]=strtok_r(i == 0 ? tmp2: 0, ":",&buf2);  
  }
  st.hour = atoi(pp[0]);
  st.minute = atoi(pp[1]);
  st.second = atoi(pp[2]);
  

  

  
  sprintf(tmp3,"%04d-%02d-%02d %02d:%02d:%02d", st.year, st.month, st.day ,st.hour,st.minute,st.second );
  printf("%s\n", tmp3);
  
  snprintf(outBuf, outBufLen -1 , "%s",tmp3);
  //printf("strlen(tmp2) : %d\n", strlen(tmp2)); //printf("snprintf : %d\n", ret);
  _addHour(&st, 0);
  
  return 0;
}

int main(int argc, char* argv[]) {
  char buf[1024];
  //formatGMTToLocal("Jan 11 21:51:35.490977 2021 GMT", buf, sizeof(buf));printf("buf : [%s]\n", buf);
  
  _addHour("Jan 11 21:51:35.490977 2021 GMT", "%b %d %H:%M:%S.%g%g%g %Y %Z", 0);
  return 0;
}


