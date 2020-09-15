#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <stdarg.h>
#include <getopt.h>



#include <unistd.h>
#include <errno.h>
#include <sys/file.h>

  
static std::string __format(const char *fmtTemplate, ...)
{
  va_list ap;
  va_start(ap, fmtTemplate);
  char buf[4096];
  int rv;
  rv = vsnprintf(buf, sizeof(buf), fmtTemplate, ap);

  if (rv < 0) {
    buf[0] = '\0';
  }

  va_end(ap);
  return buf;
}


static std::string getSelfPath(){
  char current_absolute_path[256] = {0};
  //获取当前程序绝对路径
  int cnt = readlink("/proc/self/exe", current_absolute_path, sizeof(current_absolute_path));
  if (cnt < 0 || cnt >= sizeof(current_absolute_path))
  {
    printf("*** readlink Error***\n");
    exit(-1);
  }
  return current_absolute_path;
}

static std::string getExeName(){
  char current_absolute_path[256] = {0};
  //获取当前程序绝对路径
  int cnt = readlink("/proc/self/exe", current_absolute_path, sizeof(current_absolute_path));
  if (cnt < 0 || cnt >= sizeof(current_absolute_path))
  {
    printf("*** readlink Error***\n");
    exit(-1);
  }
  //获取当前目录绝对路径，即去掉程序名
  
  int i = 0;
  for (i = cnt; i >=0; --i)
  {
    if (current_absolute_path[i] == '/')
    {
        current_absolute_path[i] = '\0';
        break;
    }
  }
  return current_absolute_path + i + 1;
  
}

static int call_subProc(int isChild)
{
	int ret = 0;
  
  int    status;
  pid_t pid;
  int pipe_fd[2];
  char acFd1[32] = {0};
  char acFd2[32] = {0};
  char r_buf[2048] = {0};

  if(pipe(pipe_fd)<0)
  {
    printf("pipe create error \n");
    return -1;
  }
  snprintf(acFd1, sizeof(acFd1), "%d", pipe_fd[0]);
  snprintf(acFd2, sizeof(acFd2), "%d", pipe_fd[1]);
   
    
    if ((pid = fork()) < 0)
    {
      return -1;
    }     
    else if (pid == 0)
    { 
      close(pipe_fd[1]);
      seteuid(getuid());
      std::string cmd = __format("%s" ,getSelfPath().c_str());
      //子进程 
      execl(cmd.c_str(), "xxxxxx", (char *)0);
      _exit(127);
    }else{
      if(waitpid(pid, &status, 0) < 0)
      {
        printf("waitpid failed\n");
        goto err;
      }  
    }
    
    
    if (WIFEXITED(status)) {
       printf("exited, status=%d\n", WEXITSTATUS(status));
       if( WEXITSTATUS(status) == 0){
          // process normal exist
          printf("start read data...\n");
          ret = read(pipe_fd[0], r_buf, sizeof(r_buf));
          if(ret > 0)
          { 
           
          }
          printf("end read data...\n");
          
          ret = 0;
          goto err;
       }
    } else if (WIFSIGNALED(status)) {
       printf("killed by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
       printf("stopped by signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
       printf("continued\n");
    }
    ret = -1;
    

err:
  close(pipe_fd[1]);
  close(pipe_fd[0]);
    
    
  return ret;
}


static int proc_detect()
{
    char filename[100] = {0};
    std::string proc = getSelfPath();;
    sprintf(filename, "%s.pid", proc.c_str());
    
    int fd = open(filename, O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    if (fd < 0) {
        printf("open file \"%s\" failed!!!\n", filename);
        return 1;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;

    int ret = fcntl(fd, F_SETLK, &fl);
    if (-1 == ret) {
        printf("file \"%s\" locked. proc already exit!!!\n", filename);
        close(fd);
        return 1;
    } else {
        ftruncate(fd, 0);
        char buf[16];
        sprintf(buf, "%ld", (long)getpid());
        write(fd, buf, strlen(buf) + 1);
        //do not close file
        return 0;
    }
}



static void mointer(int argc, char* argv[])
{
    int ret = 0;
    int is_mointor = -1;
    int is_daemon = -1;
    char short_opts[] = "d";
    int option_index = 0;
    int opt;
    struct option long_opts[] = {
        {"has-mointor-child", no_argument, 0, 0},
        {"daemon", no_argument, 0, 'd'},
        {NULL, 0, NULL, 0}
    };
    
     while ((opt = getopt_long(argc, argv, short_opts, long_opts, &option_index)) != -1) 
     {
            switch (opt) 
            {       // case为函数返回值
             case 0:
                if (strcmp((long_opts[option_index]).name , "has-mointor-child") == 0 )
                {
                    is_mointor = 1;
                }
                break;
            case 'd':
                is_daemon = 1;
            }
    }
    if (is_mointor == 1){
        printf(" hash mointor !!!!!\n");
        return ;
    }
    if(is_daemon == 1){
        if(daemon(0,1)  == -1){
            perror("daemon failed");
            exit(0);
        }   
        printf("start with daemon\n");
    }

    while(1){
        int    status = 0;
        pid_t pid;

        if ((pid = fork()) < 0)
        {
            printf("fork failed\n");
            exit(-1);
        }     
        else if (pid == 0)
        { 
             ret = proc_detect();
            	if(ret != 0){
            	    exit(110);
            	}
            //child
            seteuid(getuid());
            std::string cmd = __format("%s" ,getSelfPath().c_str());
            const char* _cmd = cmd.c_str();
            std::string exe = getExeName();
            const char* _exe = exe.c_str();
            //子进程 
            execl(_cmd, _exe, "--has-mointor-child", (char *)0);
            _exit(127);
        }else{
            if(waitpid(pid, &status, 0) < 0)
            {
                printf("waitpid failed\n");
            }else{
                
                if (WIFEXITED(status)) {
                    printf("exited, status=%d\n", WEXITSTATUS(status));
                    if( WEXITSTATUS(status) == 0){
                        // process normal exist
                        printf("process normal exist\n");
                    }
                    if(WEXITSTATUS(status) == 110){
                        printf("process has run! break\n");
                        break;
                    }
                } else if (WIFSIGNALED(status)) {
                    printf("killed by signal %d\n", WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                    printf("stopped by signal %d\n", WSTOPSIG(status));
                } else if (WIFCONTINUED(status)) {
                    printf("continued\n");
                }
                
            }
        }

        sleep(2);
    }
      
    exit(-2);
}







int main(int argc, char* argv[]){
	mointer(argc, argv);
	
	for(int i=1;i++;){
	    printf("child run [%d]\n",i);
	    sleep(2);
	    char *p = 0;
	    //*p = 0;
	}
	return 0;
}



