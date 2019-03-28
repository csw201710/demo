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


