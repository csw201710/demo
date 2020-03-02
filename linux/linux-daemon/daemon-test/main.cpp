#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include<fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>


typedef void (SERVICE_FUNCTION)();

static bool unix_svc_terminate = false;

static char ToUpper(char c)
{
	if ('a' <= c && c <= 'z')
	{
		c += 'Z' - 'z';
	}
	return c;
}

static int StrCmpi(const char *str1, const char *str2)
{
	unsigned int i;
	// Validate arguments
	if (str1 == NULL && str2 == NULL)
	{
		return 0;
	}
	if (str1 == NULL)
	{
		return 1;
	}
	if (str2 == NULL)
	{
		return -1;
	}

	// String comparison
	i = 0;
	while (true)
	{
		char c1, c2;
		c1 = ToUpper(str1[i]);
		c2 = ToUpper(str2[i]);
		if (c1 > c2)
		{
			return 1;
		}
		else if (c1 < c2)
		{
			return -1;
		}
		if (str1[i] == 0 || str2[i] == 0)
		{
			return 0;
		}
		i++;
	}
}
void GetExeName(char *exe, int size)
{
  //获取当前程序绝对路径
  int cnt = readlink("/proc/self/exe", exe, size);
  if (cnt < 0 || cnt >= 256)
  {
      printf("***readlink Error***\n");
      exit(-1);
  }
  /*
  int i;
  for(i=strlen(dir);i>0;i--)
  {
      if(*(dir+i)=='/')//*(dir+i)  == '\\'  || 
      {
          *(dir+i)  = '\0';
          break;
      }
  }
  */
}



static bool UnixIsProcess(unsigned int pid)
{
	if (getsid((pid_t)pid) == -1)
	{
		return false;
	}

	return true;
}


static unsigned int UnixReadPidFile(){
	char pidName [256] ={0};
	char pid[256] = {0};
	GetExeName(pidName, sizeof(pidName));
	strncat(pidName,".pid", 4);
	FILE *fp = fopen(pidName, "rb");
	
	if(fp && fgets(pid,sizeof(pid), fp)){
		fclose(fp);
		return strtoul(pid, NULL, 0);
	}
	return 0;
}

static void closeIo()
{
	static bool close_io_first = false;

	// Execute only once
	if (close_io_first)
	{
		return;
	}
	else
	{
		close(0);
		close(1);
		close(2);
		open("/dev/null", O_RDWR);
		dup2(0, 1);
		dup2(0, 2);
		close_io_first = true;
	}
}
static int checkIsRunning(){
	unsigned int pid;
	pid = UnixReadPidFile();
	if(pid == 0){
		return 0;
	}
	if(UnixIsProcess(pid)){
		return 1;
	}
	return 0;
}

static void UnixStartService(){
	char exe[256] = {0};
	int ret = 0;
	GetExeName(exe, sizeof(exe));
	
	ret = checkIsRunning();
	if(ret != 0){
		printf("[*] service has already running\n");
		return;
		
	}
	
	int pid;
	printf("[*] service start to running\n");
	// Create a child process
	pid = fork();
	if (pid == -1)
	{
		perror("fork service failed");
	}
	else
	{
		if (pid == 0)
		{
			// Child process
			char param[] = "exec";
			char **args;

			// Daemonize
			setsid();
			closeIo();
			signal(SIGHUP, SIG_IGN);

			// Prepare arguments
			args =(char**) calloc(1,sizeof(char *)*3);
			args[0] =(char*) exe;
			args[1] = param;
			args[2] = NULL;

			execvp(exe, args);
			abort();
		}
		else
		{
			// Don't write the child process number to the file
//				UnixWritePidFile(pid);
		}
	}
	
	
}



static bool UnixWaitProcessEx(unsigned int pid,  unsigned int timeout/*timeout 秒 */)
{
	struct timeval _start_time;
	unsigned int end_tick = timeout;
	gettimeofday( &_start_time, NULL );
	if (timeout == -1)
	{
		end_tick = 0;
	}
	while (UnixIsProcess(pid))
	{
		if (end_tick != 0)
		{
	       struct timeval end;
	       gettimeofday( &end, NULL );
	       float time_use=(end.tv_sec-_start_time.tv_sec)*1000+(end.tv_usec-_start_time.tv_usec)/1000;//毫秒
			if (end_tick * 1000 < time_use)
			{
				return false;
			}
		}
		usleep(1000);
	}
	return true;
}


static void UnixStopService(){
	char exe[256];
	unsigned int pid;
	GetExeName(exe, sizeof(exe));
	pid = UnixReadPidFile();
	if(pid == 0){
		printf("[*] Service is not running yet\n");
		return;
	}
	
	int status;
	// Stop the service
	printf("[*] Stop the [%u] service\n",pid);

	// Terminate the process
	kill(pid, SIGTERM);

	if (UnixWaitProcessEx(pid, 3))
	{
		printf("[*] Stop the service finished\n");
	}
	else
	{
		// SIGKILL
		char tmp[256];

		sprintf(tmp, "kill  %u", pid);
		printf("[*] Stop the service failed\n");
		system(tmp);
	}
	
	
}
void UnixWritePidFile(unsigned int pid)
{
	char pidName [256] ={0};
	GetExeName(pidName, sizeof(pidName));
	strncat(pidName,".pid", 4);
	FILE *fp = fopen(pidName, "wb");
	if(fp == 0){
		perror("open pidFile failed");
		return;
	}
	fprintf(fp, "%u", pid);
	fclose(fp);
	
}


// Handler of the stop signal to the process
static void UnixSigTermHandler(int signum)
{
	if (signum == SIGTERM)
	{
		unix_svc_terminate = true;
	}
}
static void UnixDeletePidFile()
{
	char pidName [256] ={0};
	GetExeName(pidName, sizeof(pidName));
	strncat(pidName,".pid", 4);
	unlink(pidName);
}

static void * __pthread(void *arg){
	SERVICE_FUNCTION* start = (SERVICE_FUNCTION*) arg;
	start();
}

static void runpthread(SERVICE_FUNCTION  arg)
{
	pthread_t tidp;
	if(pthread_create(&tidp, NULL, __pthread, (void*)arg) == -1){
		 perror("create thread!\n");
		 exit(-1);
	}
}
static void UnixExecService( SERVICE_FUNCTION *start, SERVICE_FUNCTION *stop)
{

	UnixWritePidFile(getpid());
	
	runpthread(start);

	signal(SIGTERM, &UnixSigTermHandler);
	signal(SIGINT, &UnixSigTermHandler);
	
	while (unix_svc_terminate == false)
	{
		sleep(2);
	}	
	stop();
	UnixDeletePidFile();
}
static void UnixServiceMain(int argc, char *argv[],  SERVICE_FUNCTION *start, SERVICE_FUNCTION *stop)
{
	int mode = 0;
	if (argc >= 2)
	{
		if (StrCmpi(argv[1], "start") == 0)
		{
			mode = 1;
		}
		if (StrCmpi(argv[1], "stop") == 0)
		{
			mode = 2;
		}
		if (StrCmpi(argv[1], "exec") == 0)
		{
			mode = 3;
		}
		if (StrCmpi(argv[1], "exit") == 0)
		{
			mode = 4;
		}
	}
	switch (mode)
	{

	case 1:
		UnixStartService();
		break;

	case 2:
		UnixStopService();	
		break;

	case 3:
		UnixExecService(start, stop);
		break;
	case 4:
		break;
	default:
		printf("usage: \n"
				"	start start program\n"
				"	stop  stop program\n"
				"	exec  exec program\n"
		);
		break;
	}

}

int UnixService(int argc, char *argv[], SERVICE_FUNCTION *start, SERVICE_FUNCTION *stop)
{
	if (start == NULL || stop == NULL)
	{
		return 0;
	}
	if (argc >= 2 && strcmp(argv[1], "exec") == 0)
	{
		pid_t pid;
		// Start a child process
		// Restart if the child process didn't exit properly

RESTART_PROCESS:
		pid = fork();
		if ((int)pid != -1)
		{
			if (pid == 0)
			{
				// Run the main process
				//printf("[child process]\n");
				UnixServiceMain(argc, argv, start, stop);
			}
			else
			{
				int status = 0, ret;
				printf("[parent process]\n");
				// Wait for the termination of the child process
			#if 1
				ret = waitpid(pid, &status, 0);
				printf("[parent process] waitpid return\n");
				//指出子进程是否为正常退出的，如果是，它会返回一个非零值
				if (WIFEXITED(status) == 0)
				{
					// Aborted
					//UnixSleep(100);
					sleep(1);
					printf("[*] [no use] Aborted restart ...\n");
					//goto RESTART_PROCESS;
				}
			#endif
			}
		}
	}else{
		UnixServiceMain(argc, argv, start, stop);
	}

	return 0;
}

void StartProcess()
{
	printf("%s doing\n",__func__);
	while(1){
		sleep(1);
		printf("[child process]\n");
	}
}

// Process termination function
void StopProcess()
{
	printf("%s doing\n",__func__);
}


int main(int argc,char** argv){
	UnixService(argc, argv, StartProcess, StopProcess);

	return 0;
}
