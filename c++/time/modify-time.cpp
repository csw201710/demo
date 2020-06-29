#include <stdio.h>
#include <time.h>
#include <iostream>



time_t tm_to_timet(struct tm timeDetail){
  time_t timeResult = 0;
  if( (timeResult = mktime( &timeDetail )) != (time_t)-1 )
  {
    return timeResult;
  }else{
    return 0;
  }
}
time_t convertTime(const char* strTime){
  tm tm_  = {0};  
  time_t t_ = 0;  
  char buf[128] = {0};
  strptime(strTime, "%Y-%m-%d %H:%M:%S", &tm_);
  
  t_ = tm_to_timet(tm_);
    
  tm_ = *localtime(&t_);
  strftime(buf, 64, "%Y-%m-%d %H:%M:%S", &tm_);  
  std::cout << buf << std::endl;  
  
  return t_;
  
}

int changeServerTime(time_t secs){
    if(stime(&secs)<0){
      perror("stime failed");
      return -1;
  }
  return 0;
}
// [exe] "2020-01-02 09:08:01"
int main(int argc, char** argv){
  int ret = -1;
  if(argc <= 1){
    printf("argv not enough\n");
    return -1;
  }
  char * buf = argv[1];
  
  time_t t = convertTime(buf);
  printf("t : %d\n", t);
  ret = changeServerTime(t);
  if(ret != 0){
    printf("changeServerTime failed!\n");
    return -1;
  }
  printf("changeServerTime ok!\n");

  return 0;

}

