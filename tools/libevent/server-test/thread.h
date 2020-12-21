
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <event.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <grp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/param.h>
#include <sys/resource.h>
#include <sys/uio.h>
#include <ctype.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <assert.h>
#include <limits.h>


#include "queue.h"

typedef enum {
    init_state = 0,
    terminal_state = 1
}state_t;

typedef struct conn conn;
struct conn{
    QUEUE head;
    int sfd; //socket
    struct event* event;
#ifdef TLS
    int ssl_enabled;
    void *ssl;
#endif
    state_t state;
    struct bufferevent *bev;
    char ip[64];
    int port;
};






void memcached_thread_init(int nthreads, void *arg);
void stop_threads(void);

void dispatch_conn_new(int sfd,  int event_flags, void *ssl);

void event_handler(const evutil_socket_t fd, const short which, void *arg);
conn* client_new(void* ctx, int fd, struct bufferevent *bev);
conn* server_new(int port);
void conn_close(conn *c);
