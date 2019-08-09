// 不同进程 或者 同一进程多线程 对文件加锁 ok
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include <errno.h>
#include <pthread.h>

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/file.h>

void sys_err(const char*str)
{
    perror(str);
    return;
}
 
#if 0

int io_openFile(const char *pathname, int flags){
  
  int fd = 0;
    if( ( fd = open("123.txt",flags)) < 0 )
        return -1;

    if((flock(fd, LOCK_EX | LOCK_NB))< 0){
      printf("get flock failed\n");  
      close(fd);
      return -1;
    }
    printf("get flock\n");
    return fd;
}

void io_closeFile(int fd){
     
  if(fd){
    
    if((flock(fd, LOCK_UN ))<0){
      printf("unflock failed\n");  
    }
    printf("unflock ok\n");  
     close(fd);
    
  }
}
 
void* creat1(void* arg)
{
    int fd;
 
 

     if( ( fd = io_openFile("123.txt",O_RDWR)) < 0 ){
      sys_err("open");
      return 0;
     }
        

    //f_lock.l_type = F_WRLCK;          // 选用写锁 进程会阻塞， 指定的文件被修改后，会导致新建的进程直接获取锁，尽管原进程正在占用锁
    //f_lock.l_type = F_RDLCK;            // 选用读锁 进程不会阻塞
    //f_lock.l_whence = 0;
    //f_lock.l_len = 0;                 // 0 表示整个文件加锁
 
    //fcntl(fd,F_SETLKW,&f_lock);
    //printf("get flock\n");
    
 
    sleep(10);

 
    io_closeFile(fd);
    return 0;
}

#else

FILE * io_openFile(const char *path, const char *mode){
  int ret = 0;
   FILE* fp = fopen("123.txt",mode); 
   if(fp == 0){
    perror("fopen failed!");
    return 0;
  }
  
  int fd = fileno(fp);
  if((flock(fd, LOCK_EX | LOCK_NB))< 0){
    //perror("get flock failed\n");  
    close(fd);
    return 0;
  }
  //printf("get flock\n");
  
  return fp;
}

void io_closeFile(FILE *fp){
  if(fp){
    int fd = fileno(fp);
    if((flock(fd, LOCK_UN ))<0){
      printf("unflock failed\n");  
    }
    printf("unflock ok\n"); 
    fclose(fp);
  }
}


void *creat1(void *arg)
{
  FILE* fp = io_openFile("123.txt","wb+");
  if(fp == 0){
    printf("[thread %s] io_openFile failed\n",arg);
    return 0;
  }
  printf("[thread %s]io_openFile ok\n", arg);
  
  sleep(5);
  
  printf("[thread %s]io_closeFile\n", arg);
  io_closeFile(fp);
  return 0;

}


#endif

int main(){
        pthread_t trd;
      pthread_t trd2;
      pthread_t trd3;
    pthread_create(&trd,NULL,creat1, (void*)"1");
    pthread_create(&trd2,NULL,creat1, (void*)"2");
    
      pthread_join(trd,NULL);
  pthread_join(trd2,NULL);
 // pthread_join(trd3,NULL);
}



