#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

#define _PATH_DEMON_PID "/var/run/example_0x01_.pid"


/*static int trylock_file(int fd) {
	struct flock fl;

	memset(&fl, '\0', sizeof (fl));
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	return fcntl(fd, F_SETLK, &fl);
}
*/
//# define trylock_file(fd)      lockf((fd), F_TLOCK, 0)

/*
LOCK_EX  无论写入/读取都需要等待
LOCK_NB，当被锁定时，不阻塞，而是提示锁定。
*/
# define trylock_file(fd)      flock((fd), LOCK_EX|LOCK_NB)



void acquire_daemonlock(int closeflag)
{
    static int fd = -1;
    char buf[3 * 256];
    const char *pidfile;
    char *ep;
    long otherpid = -1;
    ssize_t num, len;
    pid_t pid = getpid();

    if (closeflag)
    {
        /* close stashed fd for child so we don't leak it. */
        if (fd != -1)
        {
            close(fd);
            fd = -1;
        }
        return;
    }

    if (fd == -1)
    {
        pidfile = _PATH_DEMON_PID;
        /* Initial mode is 0600 to prevent flock() race/DoS. */
        if ((fd = open(pidfile, O_RDWR | O_CREAT, 0600)) == -1)
        {
            int save_errno = errno;
            sprintf(buf, "can't open or create %s", pidfile);
            fprintf(stderr, "%s: %s\n", buf,
                    strerror(save_errno));

            exit(1);
        }

        if (trylock_file(fd) < 0)
        {
            int save_errno = errno;

            memset(buf, 0, sizeof (buf));
            if ((num = read(fd, buf, sizeof (buf) - 1)) > 0 &&
                    (otherpid = strtol(buf, &ep, 10)) > 0 &&
                    ep != buf && *ep == '\n' && otherpid != -1)
            {
                snprintf(buf, sizeof (buf),
                         "can't lock %s, otherpid may be %ld", pidfile, otherpid);
            }
            else
            {
                snprintf(buf, sizeof (buf),
                         "can't lock %s, otherpid unknown", pidfile);
            }
            fprintf(stderr, "%s: %s\n",  buf,
                    strerror(save_errno));
            exit(1);
        }
        (void) fchmod(fd, 0644);
        (void) fcntl(fd, F_SETFD, 1);/* FD_CLOEXEC 当程序执行exec函数时本fd将被系统自动关闭 */
    }

    sprintf(buf, "%ld\n", (long) pid);
    (void) lseek(fd, (off_t) 0, SEEK_SET);
    len = strlen(buf);
    if ((num = write(fd, buf, len)) != len)
        fprintf(stderr, "ERROR: write() failed, errno:%d\n", errno);
    else
    {
        if (ftruncate(fd, num) == -1)
            fprintf(stderr, "ERROR: ftruncate() failed, errno:%d\n", errno);
    }

    /* abandon fd even though the file is open. we need to keep
     * it open and locked, but we don't need the handles elsewhere.
     */
}

char * ProgramName;
int main(int argc,char** argv)
{

    if ((ProgramName=strrchr(argv[0], '/')) == NULL)
    {
        ProgramName = argv[0];
    }
    else
    {
        ++ProgramName;
    }
    printf("%s\n",ProgramName);
    
    acquire_daemonlock(0);
    
    while(1){
      sleep(20);
    }
    return 0;

}



