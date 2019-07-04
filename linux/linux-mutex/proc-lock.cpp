/*
    互斥量 实现 多进程 之间的同步 
*/
 
#include<unistd.h>
#include<sys/mman.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
 
struct mt
{
    int num;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
};
 
 
int main(void)
{
    
    int i;
    struct mt* mm;
 
    pid_t pid;
 
    /*
    // 创建映射区文件
    int fd = open("mt_test",O_CREAT|O_RDWR,0777);
    if( fd == -1 ) 
    {
        perror("open file:"); 
        exit(1); 
    }
    ftruncate(fd,sizeof(*mm));
    mm =(mt*) mmap(NULL,sizeof(*mm),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    unlink("mt_test");
  */
 
    // 建立映射区
    mm =(mt*) mmap(NULL,sizeof(*mm),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANON,-1,0);
 
//    printf("-------before memset------\n");
    memset(mm,0x00,sizeof(*mm));
//   printf("-------after memset------\n");
 
    pthread_mutexattr_init(&mm->mutexattr);         // 初始化 mutex 属性
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);               // 修改属性为进程间共享
 
    pthread_mutex_init(&mm->mutex,&mm->mutexattr);      // 初始化一把 mutex 锁
    
    pid = fork();
    if( pid == 0 )          // 子进程
    {
        for( i=0; i<10;i++ )
        {
            pthread_mutex_lock(&mm->mutex);
            (mm->num)++;
            printf("-child--------------num++    %d\n",mm->num);
            pthread_mutex_unlock(&mm->mutex);
            sleep(1);
        }
    
    }
    else 
    {
        for( i=0;i<10;i++)
        {
            sleep(1); 
            pthread_mutex_lock(&mm->mutex);
            mm->num += 2;
            printf("--------parent------num+=2   %d\n",mm->num);
            pthread_mutex_unlock(&mm->mutex);
        
        }
        wait(NULL);
 
    }
    pthread_mutexattr_destroy(&mm->mutexattr);  // 销毁 mutex 属性对象
    pthread_mutex_destroy(&mm->mutex);          // 销毁 mutex 锁
 
    return 0;
}




#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
 
void sys_err(char*str)
{
    perror(str);
    exit(1);
}
 
 
int main(int argc,char *argv[])
{
    int fd;
    struct flock f_lock;
 
    if( argc< 2 )
    {
        printf("./a.out filename\n");
        exit(1);
    }
 
    if( ( fd = open(argv[1],O_RDWR)) < 0 )
        sys_err("open");
 
    f_lock.l_type = F_WRLCK;          // 选用写锁 进程会阻塞， 指定的文件被修改后，会导致新建的进程直接获取锁，尽管原进程正在占用锁
    //f_lock.l_type = F_RDLCK;            // 选用读锁 进程不会阻塞
    f_lock.l_whence = 0;
    f_lock.l_len = 0;                 // 0 表示整个文件加锁
 
    fcntl(fd,F_SETLKW,&f_lock);
    printf("get flock\n");
 
    sleep(20);
 
    f_lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&f_lock);
    printf("un flock\n");
 
    close(fd);
    return 0;
}
























