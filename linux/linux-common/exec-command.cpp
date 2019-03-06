static int shellexec(const char *cmd)
{
		char tmp[1024];
    INFO("shexec, cmd: %s", cmd);
    FILE *pp = popen(cmd, "r");
    if(!pp) {
        INFO("error, cannot popen cmd: %s", cmd);
        return -1;
    }
    int i = 0;
    
    while(fgets(tmp, sizeof(tmp), pp) != NULL) {
        INFO("%d.get return results: %s", i, tmp);
    }

    int rv = pclose(pp);
    INFO("ifexited: %d", WIFEXITED(rv));

    if (WIFEXITED(rv))  // 正常退出的,它会返回一个非零值。
    {  
        INFO("subprocess exited, exit code: %d", WEXITSTATUS(rv));  
        return 1;
    }else{
    	ERROR("shexec failed! ifexited: %d", WIFEXITED(rv));
    	return -1;
    }
}

