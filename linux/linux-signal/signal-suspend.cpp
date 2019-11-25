#include <unistd.h>
#include <signal.h>
#include <stdio.h>
void handler(int sig) //信号处理程序
{

if(sig == SIGINT) printf("SIGINT sig\n");

else if(sig == SIGQUIT) printf("SIGQUIT sig\n");

else if(sig == SIGUSR1) printf("SIGUSR1 sig\n");
else printf("unknow sig : %d\n", sig);

}

int main()

{
sigset_t new1,old,wait; //三个信号集
struct sigaction act;
act.sa_handler = handler;

sigemptyset(&act.sa_mask);

act.sa_flags = 0;

sigaction(SIGINT, &act, 0); //可以捕捉以下三个信号：SIGINT/SIGQUIT/SIGUSR1

sigaction(SIGQUIT, &act, 0);

setproctitle("%s","this is a test");

sigaction(SIGUSR1, &act, 0);

sigemptyset(&new1);

sigaddset(&new1, SIGINT); //SIGINT信号加入到new信号集中

sigemptyset(&wait);

//sigaddset(&wait, SIGUSR1); //SIGUSR1信号加入wait

sigprocmask(SIG_BLOCK, &new1, &old); //将SIGINT阻塞，保存当前信号集到old中

//临界区代码执行

if(sigsuspend(&wait) != -1) //程序在此处挂起；用wait信号集替换new信号集。即：过来SIGUSR1信号，阻塞掉，程序继续挂起；过来其他信号，例如SIGINT，则会唤醒程序。执行sigsuspend的原子操作。注意：如果“sigaddset(&wait, SIGUSR1);”这句没有，则此处不会阻塞任何信号，即过来任何信号均会唤醒程序。

	printf("sigsuspend error");


return 0;

}

