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
		ftruncate(fd, 0); //写入运行实例的pid 
		sprintf(buf, "%ld", (long)getpid());
		write(fd, buf, strlen(buf) + 1);
		return 0;
	}
}




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




