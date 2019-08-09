
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include <errno.h>
#include <pthread.h>

/*
// 没有作用  ... 同一个进程下的多线程 无效。
FILE * io_openFile(const char *path, const char *mode){
  int ret = 0;
   FILE* fp = fopen("123.txt",mode); 
   if(fp == 0){
    perror("fopen failed!");
    return 0;
  }
 #if 1
  ret = ftrylockfile(fp) ;
  printf("ftrylockfile: %d\n",ret);
  ret = ftrylockfile(fp) ;
  printf("ftrylockfile: %d\n",ret);
  if(ret != 0){
    perror("ftrylockfile failed!");
    return 0;
  }
 #else
  flockfile(fp);
 #endif
  return fp;
}

void io_closeFile(FILE *fp){
  if(fp){
    funlockfile(fp);
    fclose(fp);
  }
}


void *creat1(void *arg)
{
  FILE* fp = io_openFile("123.txt","wb+");
  if(fp == 0){
    printf("io_openFile failed\n");
    return 0;
  }
  printf("[thread %s]io_openFile ok\n", arg);
  
  sleep(5);
  
  printf("[thread %s]io_closeFile\n", arg);
  io_closeFile(fp);
  return 0;

}

int main(){
      pthread_t trd;
      pthread_t trd2;
      pthread_t trd3;
  pthread_create(&trd,NULL,creat1, (void*)"1");
  pthread_create(&trd2,NULL,creat1, (void*)"2");
  //pthread_create(&trd3,NULL,creat1, (void*)"2");
  
  pthread_join(trd,NULL);
  pthread_join(trd2,NULL);
 // pthread_join(trd3,NULL);
  return 0;
}
*/



// 同一个进程下的多个线程是无效
int lock_set(int fd, int type)
{
  struct flock old_lock, lock;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;
  lock.l_type = type;
  lock.l_pid = -1;
  //* 判断文件是否可以上锁 
  fcntl(fd, F_GETLK, &lock);
  if (lock.l_type != F_UNLCK)
  {
    //* 判断文件不能上锁的原因 
    if (lock.l_type == F_RDLCK) //* 该文件已有读取锁 
    {
      printf("Read lock already set by %d\n", lock.l_pid);
    }
    else if (lock.l_type == F_WRLCK) //* 该文件已有写入锁 
    {
      printf("Write lock already set by %d\n", lock.l_pid);
    }
  }
  //* l_type 可能已被F_GETLK修改过 
  lock.l_type = type;
  //* 根据不同的type值进行阻塞式上锁或解锁 
  if ((fcntl(fd, F_SETLKW, &lock)) < 0)
  {
    printf("Lock failed:type = %d\n", lock.l_type);
    return 1;
  }
  switch(lock.l_type)
  {
    case F_RDLCK:
    {
      printf("Read lock set by %d\n", getpid());
    }
    break;
    case F_WRLCK:
    {
      printf("Write lock set by %d\n", getpid());
    }
    break;
    case F_UNLCK:
    {
      printf("Release lock by %d\n", getpid());
    return 1;
    }
    break;
    default:
    break;
  }
  return 0;
}
  
  
  
  
int main(){
   FILE *fp = fopen("123.txt","ab+");
   if(fp == 0){
    perror("fopen failed!");
    return 0;
  }
  //int fd = fileno(fp);
  int fd;
  if ((fd = open("123.txt", O_CREAT|O_RDWR|O_APPEND, 0644)) < 0)
  {
  printf("Open fifo file error\n");
  return 0;
  }
  
  
  printf("lock file start\n");
  if( lock_set(fd, F_WRLCK) !=0){
    printf("lock_set failed!");
    return 1;
  }
   printf("lock file end\n");
    
    sleep(10);
  #if 0
   FILE *fp2 = fopen("123.txt","ab+");
   if(fp2 == 0){
    perror("fopen failed!");
    return 0;
  }
  
  printf("lock file 2 start\n");
  //int fd2 = fileno(fp2);
  
  int fd2;
  if ((fd2 = open("123.txt", O_CREAT|O_RDWR|O_APPEND, 0644)) < 0)
  {
  printf("Open fifo file error\n");
  return 0;
  }
 
  
  
  if( lock_set(fd2, F_WRLCK) !=0){
    printf("lock_set failed!");
    return 1;
  }
  printf("lock file 2 end\n");
  #endif

  
  lock_set(fd, F_UNLCK); 
  fclose(fp);
  //fclose(fp2);

  return  0;
}



/*
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
 
void sys_err(const char*str)
{
    perror(str);
    exit(1);
}
 
 
int io_openFile(const char *pathname, int flags){
  struct flock f_lock;
  int fd = 0;
    if( ( fd = open("123.txt",flags)) < 0 )
        return 0;
 
    f_lock.l_type = F_WRLCK;          // 选用写锁 进程会阻塞， 指定的文件被修改后，会导致新建的进程直接获取锁，尽管原进程正在占用锁
    //f_lock.l_type = F_RDLCK;            // 选用读锁 进程不会阻塞
    f_lock.l_whence = 0;
    f_lock.l_len = 0;                 // 0 表示整个文件加锁
 
    fcntl(fd,F_SETLKW,&f_lock);
    printf("get flock\n");
    return fd;
}

void io_closeFile(int fd){
     struct flock f_lock;
  if(fd){
    fcntl(fd, F_GETLK, &f_lock);
    f_lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&f_lock);
    printf("un flock\n");
    
  }
}
 
void* creat1(void* arg)
{
    int fd;
 
 

     if( ( fd = io_openFile("123.txt",O_RDWR)) < 0 )
        sys_err("open");

    //f_lock.l_type = F_WRLCK;          // 选用写锁 进程会阻塞， 指定的文件被修改后，会导致新建的进程直接获取锁，尽管原进程正在占用锁
    //f_lock.l_type = F_RDLCK;            // 选用读锁 进程不会阻塞
    //f_lock.l_whence = 0;
    //f_lock.l_len = 0;                 // 0 表示整个文件加锁
 
    //fcntl(fd,F_SETLKW,&f_lock);
    //printf("get flock\n");
    
 
    sleep(20);

 
    io_closeFile(fd);
    return 0;
}

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
*/



