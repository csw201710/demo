#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>  
int main ()  
{  
    pid_t fpid; //fpid表示fork函数返回的值 
    int count=0; 
    fpid=fork();  
    if (fpid < 0)  
        printf("error in fork!");  
    else if (fpid == 0) { 
        printf("i am the child process, my process id is %d\n",getpid());  
        
        count++; 
        //sleep(1);
        exit(0);
    } 
    printf("i am the parent process, my process id is %d\n",getpid());  

    count++; 

    //子进程提前退出，产生僵尸进程
    sleep(3);
    //wait(0); // waitpid
    printf("统计结果是: %d\n",count); //数据独立，count == 1
    //退出后，僵尸进程消失
    return 0; 
}  
