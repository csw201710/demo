#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <syslog.h>


#define _CHILD_DEBUG_(...) \
	printf(__VA_ARGS__); \
	syslog(LOG_INFO,__VA_ARGS__);

int
main(int argc, char **argv)
{
    int ret, i, status;
    char *child_argv[100] = {0};
    char buf[256]={0};
    pid_t pid,w;
    if (argc < 2) {
 
        fprintf(stderr, "Usage:%s <exe_path> <args...>\n", argv[0]);
        return -1;
    }
    for (i = 1; i < argc; ++i) {
        child_argv[i-1] = (char *)malloc(strlen(argv[i])+1);
        strncpy(child_argv[i-1], argv[i], strlen(argv[i]));
        child_argv[i-1][strlen(argv[i])] = '\0';
    }
    while(1){
 
        pid = fork(); 
        if (pid < -1) {
            fprintf(stderr, "fork() error.errno:%d error:%s\n", errno, strerror(errno));
            break;
        }
        if (pid == 0) {
        	  /*path参数 启动程序的名称包括路径名
        	  	arg参数  表示启动程序所带的参数，一般第一个参数为要执行命令名，
        	  	         不是带路径且arg必须以NULL结束
        	  */
            ret = execv(child_argv[0], (char **)child_argv);
            if (ret < 0) {
                fprintf(stderr, "execv ret:%d errno:%d error:%s\n", ret, errno, strerror(errno));
            }
            exit(0);
        }
        w = waitpid(pid,&status,0);
        fprintf(stdout, "%s waitpid return. status:%d\n",argv[1],status);
        syslog(LOG_INFO,"%s waitpid return. status:%d\n",argv[1],status);        
       if (WIFEXITED(status)) {
           _CHILD_DEBUG_("%s exited, status=%d\n",argv[1], WEXITSTATUS(status));
       } else if (WIFSIGNALED(status)) {
           _CHILD_DEBUG_("%s killed by signal %d\n" ,argv[1], WTERMSIG(status));
       } else if (WIFSTOPPED(status)) {
           _CHILD_DEBUG_("%s stopped by signal %d\n",argv[1], WSTOPSIG(status));
		       sprintf(buf,"kill -9 %d",pid);
		       system(buf);
       } else if (WIFCONTINUED(status)) {
           _CHILD_DEBUG_("%s continued\n", argv[1]);
		       sprintf(buf,"kill -9 %d",pid);
		       system(buf);           
       }
        sleep(1);
 
    }
 
 
    return 0;
}

