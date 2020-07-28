#include <unistd.h>
#include <stdio.h>
#include <signal.h>


/*这段函数的执行时间不计算在for循环的时间之内*/
void handler(int a) {
	printf("Hello\n");
	alarm(5); //[ok] 单次触发，需要手动重置
}

int main()
{
	int i;
	signal(SIGALRM,handler);
	alarm(5);
	for(i=1;i<21;i++){
		printf("sleep %d ...\n",i);
		sleep(1);
	}
return 0;
}


/*
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
 
static int alarm_fired = 0;
 
void ouch(int sig)
{
    alarm_fired = 1;
}
 
int main()
{
    // 关联信号处理函数
    signal(SIGALRM, ouch);
     
    // 调用alarm函数，5秒后发送信号SIGALRM
    alarm(5);
     
    // 挂起进程
    pause();
     
    // 接收到信号后，恢复正常执行
    if(alarm_fired == 1)
    {
        printf("Receive a signal %d\n", SIGALRM);
    }
 
    exit(0);
}
*/


