
// SocketLineReader wraps a small buffer around a socket for line-orientated
// protocols.
class SocketLineReader {
 public:
  explicit SocketLineReader(int sock) : sock_(sock) {}

  // Next reads a '\n'- or '\r\n'-terminated line from the socket and, on
  // success, sets |*out_line| to it and returns true. Otherwise it returns
  // false.
  bool Next(std::string *out_line) {
    for (;;) {
      for (size_t i = 0; i < buf_len_; i++) {
        if (buf_[i] != '\n') {
          continue;
        }

        size_t length = i;
        if (i > 0 && buf_[i - 1] == '\r') {
          length--;
        }

        out_line->assign(buf_, length);
        buf_len_ -= i + 1;
        OPENSSL_memmove(buf_, &buf_[i + 1], buf_len_);

        return true;
      }

      if (buf_len_ == sizeof(buf_)) {
        fprintf(stderr, "Received line too long!\n");
        return false;
      }

      ssize_t n;
      do {
        n = recv(sock_, &buf_[buf_len_], sizeof(buf_) - buf_len_, 0);
      } while (n == -1 && errno == EINTR);

      if (n < 0) {
        fprintf(stderr, "Read error from socket\n");
        return false;
      }

      buf_len_ += n;
    }
  }

  // ReadSMTPReply reads one or more lines that make up an SMTP reply. On
  // success, it sets |*out_code| to the reply's code (e.g. 250) and
  // |*out_content| to the body of the reply (e.g. "OK") and returns true.
  // Otherwise it returns false.
  //
  // See https://tools.ietf.org/html/rfc821#page-48
  bool ReadSMTPReply(unsigned *out_code, std::string *out_content) {
    out_content->clear();

    // kMaxLines is the maximum number of lines that we'll accept in an SMTP
    // reply.
    static const unsigned kMaxLines = 512;
    for (unsigned i = 0; i < kMaxLines; i++) {
      std::string line;
      if (!Next(&line)) {
        return false;
      }

      if (line.size() < 4) {
        fprintf(stderr, "Short line from SMTP server: %s\n", line.c_str());
        return false;
      }

      const std::string code_str = line.substr(0, 3);
      char *endptr;
      const unsigned long code = strtoul(code_str.c_str(), &endptr, 10);
      if (*endptr || code > UINT_MAX) {
        fprintf(stderr, "Failed to parse code from line: %s\n", line.c_str());
        return false;
      }

      if (i == 0) {
        *out_code = code;
      } else if (code != *out_code) {
        fprintf(stderr,
                "Reply code varied within a single reply: was %u, now %u\n",
                *out_code, static_cast<unsigned>(code));
        return false;
      }

      if (line[3] == ' ') {
        // End of reply.
        *out_content += line.substr(4, std::string::npos);
        return true;
      } else if (line[3] == '-') {
        // Another line of reply will follow this one.
        *out_content += line.substr(4, std::string::npos);
        out_content->push_back('\n');
      } else {
        fprintf(stderr, "Bad character after code in SMTP reply: %s\n",
                line.c_str());
        return false;
      }
    }

    fprintf(stderr, "Rejected SMTP reply of more then %u lines\n", kMaxLines);
    return false;
  }

 private:
  const int sock_;
  char buf_[512];
  size_t buf_len_ = 0;
};



// SendAll writes |data_len| bytes from |data| to |sock|. It returns true on
// success and false otherwise.
static bool SendAll(int sock, const char *data, size_t data_len) {
  size_t done = 0;

  while (done < data_len) {
    ssize_t n;
    do {
      n = send(sock, &data[done], data_len - done, 0);
    } while (n == -1 && errno == EINTR);

    if (n < 0) {
      fprintf(stderr, "Error while writing to socket\n");
      return false;
    }

    done += n;
  }

  return true;
}











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



