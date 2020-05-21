//socket获取一行
int get_line(int sock, char *buf, int size)
{
 int i = 0;
 char c = '\0';
 int n;

 while ((i < size - 1) && (c != '\n'))
 {
  n = recv(sock, &c, 1, 0);
  /* DEBUG printf("%02X\n", c); */
  if (n > 0)
  {
   if (c == '\r')
   {
    n = recv(sock, &c, 1, MSG_PEEK);
    /* DEBUG printf("%02X\n", c); */
    if ((n > 0) && (c == '\n'))
     recv(sock, &c, 1, 0);
    else
     c = '\n';
   }
   buf[i] = c;
   i++;
  }
  else
   c = '\n';
 }
 buf[i] = '\0';
 
 return(i);
}





//切换低用户权限
/* lose root privileges if we have them */
if (getuid() == 0 || geteuid() == 0) {
	if (username == 0 || *username == '\0') {
		fprintf(stderr, "can't run as root without the -u switch\n");
		exit(EX_USAGE);
	}
	if ((pw = getpwnam(username)) == 0) {//获取用户登录相关信息
		fprintf(stderr, "can't find the user %s to switch to\n", username);
		exit(EX_NOUSER);
	}
	if (setgroups(0, NULL) < 0) {
		/* setgroups may fail with EPERM, indicating we are already in a
		 * minimally-privileged state. In that case we continue. For all
		 * other failure codes we exit.
		 *
		 * Note that errno is stored here because fprintf may change it.
		 */
		 //EPERM：权限不足, 必须是root 权限
		bool should_exit = errno != EPERM;
		fprintf(stderr, "failed to drop supplementary groups: %s\n",
				strerror(errno));
		if (should_exit) {
			exit(EX_OSERR);
		}
	}
	if (setgid(pw->pw_gid) < 0 || setuid(pw->pw_uid) < 0) {
		fprintf(stderr, "failed to assume identity of user %s\n", username);
		exit(EX_OSERR);
	}
}



//后台进程
#ifndef HAVE_SIGIGNORE
static int sigignore(int sig) {
    struct sigaction sa = { .sa_handler = SIG_IGN, .sa_flags = 0 };

    if (sigemptyset(&sa.sa_mask) == -1 || sigaction(sig, &sa, 0) == -1) {
        return -1;
    }
    return 0;
}
#endif

int daemonize(int nochdir, int noclose)
{
    int fd;

    switch (fork()) {
    case -1:
        return (-1);
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
        return (-1);

    if (nochdir == 0) {
        if(chdir("/") != 0) {
            perror("chdir");
            return (-1);
        }
    }

    if (noclose == 0 && (fd = open("/dev/null", O_RDWR, 0)) != -1) {
        if(dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 stdin");
            return (-1);
        }
        if(dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 stdout");
            return (-1);
        }
        if(dup2(fd, STDERR_FILENO) < 0) {
            perror("dup2 stderr");
            return (-1);
        }

        if (fd > STDERR_FILENO) {
            if(close(fd) < 0) {
                perror("close");
                return (-1);
            }
        }
    }
    return (0);
}


if (do_daemonize) {
	if (sigignore(SIGHUP) == -1) {
		perror("Failed to ignore SIGHUP");
	}
	if (daemonize(maxcore, settings.verbose) == -1) {
		fprintf(stderr, "failed to daemon() in order to daemonize\n");
		exit(EXIT_FAILURE);
	}
}



