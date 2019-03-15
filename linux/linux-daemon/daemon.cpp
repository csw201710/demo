#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <fcntl.h>




#define USE_SELF_DAEMON

#define _CHILD_DEBUG_(...) \
	syslog(LOG_INFO,__VA_ARGS__); \
	fprintf(stderr,__VA_ARGS__);


/*
#define USE_SINGLETON_INSTANCE
static int lockfile(int fd) {
	struct flock fl; 
	fl.l_type = F_WRLCK; 
	fl.l_start = 0; 
	fl.l_whence = SEEK_SET; 
	fl.l_len = 0; 
	return(fcntl(fd, F_SETLK, &fl)); 
} 

static int proc_is_exist(const char *procname) {
	 int fd; char buf[16]; 
	 char filename[100]; 
	 sprintf(filename, "/var/run/%s.pid", procname); 
	 fd = open(filename, O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)); 
	 if (fd < 0) {
	 	 fprintf(stderr,"open file \"%s\" failed!!!\n", filename);
	 	  return 1;
	} 
	if (lockfile(fd) == -1) {
		 // 尝试对文件进行加锁 
		fprintf(stderr,"file \"%s\" locked. proc already exit!!!\n", filename);
		close(fd);
		return 1;
	} else { 
		ftruncate(fd, 0); // 写入运行实例的pid 
		sprintf(buf, "%ld", (long)getpid());
		write(fd, buf, strlen(buf) + 1);
		return 0;
	}
}

#ifdef USE_SINGLETON_INSTANCE
int main(int argc, char* argv[]) {

    if(proc_is_exist("helloworld") != 0){
    	fprintf(stderr,"program has running\n");
    	exit(10) ;
    }
    while(1){
    	printf("test is running...\n");
    	sleep(2);
    }
		return 0;

 }

#endif
*/   







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
 
    
#ifdef USE_SELF_DAEMON
	//忽略终端I/O信号，STOP信号
	signal(SIGTTOU,SIG_IGN);// 后台程序尝试写操作	
	signal(SIGTTIN,SIG_IGN);// 后台程序尝试读操作	
	signal(SIGTSTP,SIG_IGN);//使进程暂停
	signal(SIGHUP,SIG_IGN);// 连接挂断 
	
	pid = fork();
	if(pid > 0) {
		exit(0); //结束父进程，使得子进程成为后台进程
	}
	else if(pid < 0) { 
		_CHILD_DEBUG_("self daemon fork failed!\n");
		return -1;
	}
	//建立一个新的进程组,在这个新的进程组中,子进程成为这个进程组的首进程,以使该进程脱离所有终端
	setsid();
	//关闭所有从父进程继承的不再需要的文件描述符
	//for(i=0;i< NOFILE;close(i++));//define NOFILE 256
	
	//改变工作目录，使得进程不与任何文件系统联系
	chdir("/tmp");
	
	umask(0);
	
	//忽略SIGCHLD信号,导致父进程不能正确获取到子进程退出状态。
	//signal(SIGCHLD,SIG_IGN); //子进程结束时, 父进程会收到这个信号
	
#endif
    
    
    
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
				exit(errno);
            }
            exit(0);
        }
        w = waitpid(pid,&status,0);

       if (WIFEXITED(status)) {
           _CHILD_DEBUG_("%s exited, status=%d\n",argv[1], WEXITSTATUS(status));
           if(WEXITSTATUS(status) == 10){//demo
           	 // 多实例启动
           	 _CHILD_DEBUG_("%s child process has running,return\n",argv[1]);
           	 break;
           }
       } else if (WIFSIGNALED(status)) {
           _CHILD_DEBUG_("%s killed by signal %d\n" ,argv[1], WTERMSIG(status));
       } else if (WIFSTOPPED(status)) {
       	   //kill -s  SIGINT 79630
           _CHILD_DEBUG_("%s stopped by signal %d\n",argv[1], WSTOPSIG(status));
		       sprintf(buf,"kill -9 %d",pid);
		       system(buf);
       } else if (WIFCONTINUED(status)) {
           _CHILD_DEBUG_("%s continued\n", argv[1]);
		       sprintf(buf,"kill -9 %d",pid);
		       system(buf);           
       }else{
           _CHILD_DEBUG_("%s unknow error\n", argv[1]);
		       sprintf(buf,"kill -9 %d",pid);
		       system(buf);                  	
       }
        sleep(1);
 
    }
 
 
    return 0;
}


