void* monitor(void *a){
	while(1) {
		int ret, i, status;
		pid_t pid;  
		printf("create child process "); 
		pid = fork();   
		if (pid == -1) {  
			printf("fork() error.errno:%d error:%s", errno, strerror(errno));  
			break;  
		}  
		if (pid == 0) {  
				//char *child_argv[2];
				char *child_argv[] = {"/bin/ls", (char *)0};
			ret = execv(child_argv[0], (char **)child_argv);  
			printf("execv return");  
			if (ret < 0) {  
				printf("execv ret:%d errno:%d error:%s", ret, errno, strerror(errno));  
				exit(-1);
			}  
			printf("exit child process");  
			exit(0);  
		}  
		if (pid > 0) {  
			printf("wait for child pid: %d", pid);
			pid = waitpid(pid,&status,0);  
			printf("Child process id: %d", pid);
			//fprintf(stdout, "wait return");  
			
		}  
	}  
}

