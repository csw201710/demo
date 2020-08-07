#include <unistd.h>
#include<stdio.h>
#include <sys/types.h>

#include<string.h>
#include <errno.h>

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>



void showId(const char* prefix){
  uid_t ruid,euid,suid;
  printf("%s\n", prefix);
  
   getresuid(&ruid,&euid,&suid);
  printf("\treal:%d\teffective:%d\tset-user:%d\n",ruid,euid,suid);
}

void changeTo(int id){
    printf("\n");
    showId("Before:");
    if( setuid(id) != 0){
      printf("[error] %s\n",strerror(errno)); 
    }
    showId("After:");
    printf("\n");
}
int main(int argc,char *argv[])
{
    changeTo(0);
    changeTo(1001);
    changeTo(0);
    return 0;
}



