#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


void int_handler(int a){
	printf("sa_handler enter ...\n");
	for(int i=0;i<5;i++){
		printf("sa_handler doing ...\n");
		sleep(2);
	}
	printf("sa_handler end ...\n");
}
int main(){
	struct sigaction newact,oldact;

	newact.sa_handler = int_handler; 
	sigemptyset(&newact.sa_mask);
	sigaddset(&newact.sa_mask, SIGINT);//[ok]在处理信号时，阻塞SIGINT信号，信号处理完毕后，才处理SIGINT信号
	newact.sa_flags = 0;
	int count = 0;
	pid_t pid = 0;

	sigaction(SIGTERM,&newact,&oldact);//原来的备份到oldact里面
	while(1){
		sleep(2);
	}
	return 0;
}

