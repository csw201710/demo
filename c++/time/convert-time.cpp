#include <stdio.h>
#include <string.h>

int formatGMTToLocal(const char* t){
  //Jan 11 01:51:35.490977 2021 GMT
  
  const char* p[5] = {0};
  char *buf;
  char tmp[1024] = {0};
  char tmp2[1024] = {0};
  const char *eng_month[] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sept","Oct","Nov","Dec"
  };
  if(t == 0 || strlen(t) == 0) {
    return -1;
  }
  
  strncpy(tmp, t, strlen(t));
  p[0]=strtok_r(tmp, " ",&buf);
  p[1] = strtok_r(NULL, " ", &buf);
  p[2] = strtok_r(NULL, " ", &buf);
  p[3] = strtok_r(NULL, " ", &buf);
  p[4] = strtok_r(NULL, " ", &buf);
  printf("p[0]:%s\n",p[0]);
  printf("p[1]:%s\n",p[1]);
  printf("p[2]:%s\n",p[2]);
  printf("p[3]:%s\n",p[3]);
  printf("p[4]:%s\n",p[4]);
  
  for(int i=0; p[2] != 0 && i<strlen(p[2]);i++){
    if(p[2][i] == '.'){
      char *t = (char*)p[2];
      t[i] = '\0';
      break;
    }
  }
  for(int i=0;p[0] !=0 && i<sizeof(eng_month);i++){
    if(strcmp(eng_month[i],p[0]) == 0){
      char *t = (char*)p[0];
      sprintf(t, "%02d", (i+1));
      break;
    }
  }
  
  printf("p[0]:%s\n",p[0]);
  printf("p[1]:%s\n",p[1]);
  printf("p[2]:%s\n",p[2]);
  printf("p[3]:%s\n",p[3]);
  printf("p[4]:%s\n",p[4]);
  sprintf(tmp2,"%s-%s-%02d %s", p[3], p[0], atoi(p[1] != 0 ? p[1]:""),p[2]);
  printf("%s\n", tmp2);
  
  return 0;
}

int main(int argc, char* argv[]) {
  formatGMTToLocal("Jan 11 01:51:35.490977 2021 GMT");
  return 0;
}

