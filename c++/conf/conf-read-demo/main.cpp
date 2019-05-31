#include "configfile.h"

#include <stdio.h>

int main(){
  char tmp[1024]={0};
  char filename[] = "1.conf";
  char  key[] = "key001";
  if (read_config_file(filename, key, tmp) > 0) {
    printf("key001: %s\n",tmp);
  }

 return 0;
}