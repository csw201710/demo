static void log_pid(char *pid_file)
{
    FILE    *f;
    pid_t   pid;

    pid = getpid();
    if ((f = fopen(pid_file, "w")) == NULL)
    {
        syslog(LOG_ERR, "PPTPD: failed to open(%s), errno=%d\n",
               pid_file, errno);
        return;
    }
    fprintf(f, "%d\n", pid);
    fclose(f);
}


