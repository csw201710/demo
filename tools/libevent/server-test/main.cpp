#include <event.h>

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

#include "thread.h"

#include "Log.h"

int stop_main_loop = 0;

static struct event_base *main_base;



static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event *signal = (struct event *)arg;
    int sig = event_get_signal(signal);
    ERROR("signal_cb: got signal %d", sig );
    if(SIGUSR1 == sig){
        event_del(signal);
        stop_main_loop = 1;
    }else if(SIGINT == sig){
        //again
        //evsignal_add(signal,NULL);
        event_del(signal);
        stop_main_loop = 1;
    }



}



int runmain(){
    int retval = 0;
    int sfd;
    conn* c;

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("failed to ignore SIGPIPE; sigaction");
        exit(-1);
    }

    /* initialize main thread libevent instance */
#if defined(LIBEVENT_VERSION_NUMBER) && LIBEVENT_VERSION_NUMBER >= 0x02000101
    /* If libevent version is larger/equal to 2.0.2-alpha, use newer version */
    struct event_config *ev_config;
    ev_config = event_config_new();
    /*
    EVENT_BASE_FLAG_NOLOCK
       不要为event_base分配锁。设置这个选项可以为event_base节省一点加锁和解锁的时间，但是当多个线程访问event_base会变得不安全
    */
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    main_base = event_base_new_with_config(ev_config);
    event_config_free(ev_config);
#else
    /* Otherwise, use older API */
    main_base = event_init();
#endif

    struct event signal_int;
    event_set(&signal_int, SIGINT, EV_SIGNAL, signal_cb, (void *)&signal_int);
    event_base_set(main_base, &signal_int);
    evsignal_add(&signal_int,NULL);

    struct event signal_usr1;
    event_set(&signal_usr1, SIGUSR1, EV_SIGNAL, signal_cb, (void *)&signal_usr1);
    event_base_set(main_base, &signal_usr1);
    evsignal_add(&signal_usr1,NULL);


    // 创建工作线程,每个线程独立的 event loop
    memcached_thread_init(2 , 0);

    c = server_new(12345);

    c->event = event_new(main_base, c->sfd,EV_READ | EV_PERSIST, event_handler, (void *)c);
    //event_set(&c->event, c->sfd, EV_READ | EV_PERSIST, event_handler, (void *)c);
    //event_base_set(main_base, &c->event);
    if (event_add(c->event, 0) == -1) {
        perror("event_add");
        return -1;
    }
    stop_main_loop = 0;
    while (!stop_main_loop) {
        if (event_base_loop(main_base, EVLOOP_ONCE) != 0) {
            retval = -1;
            break;
        }
        //if(i++>100){
         //   stop_main_loop = 1;
        //}
    }

    stop_threads();

    conn_close(c);
    event_base_free(main_base);
    return retval;
}


int main(){
    runmain();
    INFO("finished");
    return 0;
}
