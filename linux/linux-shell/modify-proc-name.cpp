#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

static char *ngx_os_argv_last;

char           **ngx_os_argv;

int
ngx_init_setproctitle()
{
    unsigned char      *p;
    size_t       size;
    unsigned int   i;

    size = 0;

    for (i = 0; environ[i]; i++) {
        size += strlen(environ[i]) + 1;
    }

    p =(unsigned char* ) calloc(size, sizeof(char));
    if (p == NULL) {
        return -1;
    }

    ngx_os_argv_last = ngx_os_argv[0];

    for (i = 0; ngx_os_argv[i]; i++) {
        if (ngx_os_argv_last == ngx_os_argv[i]) {
            ngx_os_argv_last = ngx_os_argv[i] + strlen(ngx_os_argv[i]) + 1;
        }
    }

    for (i = 0; environ[i]; i++) {
        if (ngx_os_argv_last == environ[i]) {

            size = strlen(environ[i]) + 1;
            ngx_os_argv_last = environ[i] + size;

            memcpy(p, (u_char *) environ[i], size);
            environ[i] = (char *) p;
            p += size;
        }
    }

    ngx_os_argv_last--;

    return 0;
}



void
ngx_setproctitle(const char *title)
{
    char     *p;

    ngx_os_argv[1] = NULL;
    p = (char *) strncpy(( char *) ngx_os_argv[0], ( char *) title,
                    ngx_os_argv_last - ngx_os_argv[0]) + strlen(title) + 1;;
    if (ngx_os_argv_last - (char *) p) {
        memset(p, ' ', ngx_os_argv_last - (char *) p);
    }
}

int main(int argc, char* argv[])
{
	ngx_os_argv = argv;

	ngx_init_setproctitle();
	ngx_setproctitle("this is a test");

	return 0;
}


