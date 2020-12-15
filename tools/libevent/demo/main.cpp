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

//#include "cq_item.h"


#ifndef NULL
#define NULL 0
#endif

//=================================== THREAD ===================================//

typedef struct {
    int sfd; //socket
    struct event event;
    int state;
}conn;



static int init_count = 0;
static pthread_mutex_t init_lock;
static pthread_cond_t init_cond;

typedef struct conn_queue_item CQ_ITEM;
struct conn_queue_item {
    int               sfd;
    int               event_flags;
    void*             ssl;
    conn *c;
    CQ_ITEM          *next;
};


typedef struct conn_queue CQ;
struct conn_queue {
    CQ_ITEM *head;
    CQ_ITEM *tail;
    pthread_mutex_t lock;
};

typedef struct {
    pthread_t thread_id;        /* unique ID of this thread */
    struct event_base *base;    /* libevent handle this thread uses */
    struct event notify_event;  /* listen event for notify pipe */
    int notify_receive_fd;      /* receiving end of notify pipe */
    int notify_send_fd;         /* sending end of notify pipe */
    struct conn_queue *new_conn_queue; /* queue of new connections to handle */
} LIBEVENT_THREAD;


static LIBEVENT_THREAD *threads;

/* Free list of CQ_ITEM structs */
static CQ_ITEM *cqi_freelist;
static pthread_mutex_t cqi_freelist_lock;


static void cq_init(CQ *cq) {
    pthread_mutex_init(&cq->lock, NULL);
    cq->head = NULL;
    cq->tail = NULL;
}


static CQ_ITEM *cq_pop(CQ *cq) {
    CQ_ITEM *item;

    pthread_mutex_lock(&cq->lock);
    item = cq->head;
    if (NULL != item) {
        cq->head = item->next;
        if (NULL == cq->head)
            cq->tail = NULL;
    }
    pthread_mutex_unlock(&cq->lock);

    return item;
}

static void cq_push(CQ *cq, CQ_ITEM *item) {
    item->next = NULL;

    pthread_mutex_lock(&cq->lock);
    if (NULL == cq->tail)
        cq->head = item;
    else
        cq->tail->next = item;
    cq->tail = item;
    pthread_mutex_unlock(&cq->lock);
}


static CQ_ITEM *cqi_new(void) {
    CQ_ITEM *item = NULL;
    pthread_mutex_lock(&cqi_freelist_lock);
    if (cqi_freelist) {
        item = cqi_freelist;
        cqi_freelist = item->next;
    }
    pthread_mutex_unlock(&cqi_freelist_lock);

    if (NULL == item) {
        int i;

        /* Allocate a bunch of items at once to reduce fragmentation */
        item = (CQ_ITEM*) malloc(sizeof(CQ_ITEM) * 64);
        if (NULL == item) {
            return NULL;
        }

        /*
         * Link together all the new items except the first one
         * (which we'll return to the caller) for placement on
         * the freelist.
         */
        for (i = 2; i < 64; i++)
            item[i - 1].next = &item[i];

        pthread_mutex_lock(&cqi_freelist_lock);
        item[64 - 1].next = cqi_freelist;
        cqi_freelist = &item[1];
        pthread_mutex_unlock(&cqi_freelist_lock);
    }

    return item;
}




static void cqi_free(CQ_ITEM *item) {
    pthread_mutex_lock(&cqi_freelist_lock);
    item->next = cqi_freelist;
    cqi_freelist = item;
    pthread_mutex_unlock(&cqi_freelist_lock);
}

static void readcb(struct bufferevent *bev, void *ctx)
{
    char msg_header[1024] = {0};
    size_t msg_len;

    struct evbuffer *datain;
    struct evbuffer *dataout;

    datain = bufferevent_get_input(bev);
	msg_len = evbuffer_get_length(datain);
    int n = evbuffer_copyout(datain, msg_header, sizeof(msg_header));
    printf("\n%s [%d] [%s]\n",__FUNCTION__, n, msg_header);


    dataout = bufferevent_get_output(bev);
    evbuffer_add(dataout, msg_header, n);

    //evbuffer_drain 将数据从缓冲区前面移除
    evbuffer_drain(datain, msg_len);
}
static void writecb(struct bufferevent *bev, void *ctx)
{
    printf("%s called\n",__FUNCTION__);
}
static void errorcb(struct bufferevent *bev, short error, void *ctx)
{
    printf("%s called\n",__FUNCTION__);

    if (error & BEV_EVENT_CONNECTED)
    {
    		printf("Get BEV_EVENT_CONNECTED event.\n");
    }
    else if (error & BEV_EVENT_EOF)
    {
        /* connection has been closed, do any clean up here */
        printf("connection closed.\n");
    }
    else if (error & BEV_EVENT_ERROR)
    {
        /* check errno to see what error occurred */
        printf("errorcb some other error, %s.\n",  strerror(errno));
    }
    else if (error & (BEV_EVENT_TIMEOUT|BEV_EVENT_READING))
    {
        /* must be a timeout event handle, handle it */
        printf("Read Timed out.\n");
    }
    else if (error & (BEV_EVENT_TIMEOUT|BEV_EVENT_WRITING))
    {
        /* must be a timeout event handle, handle it */
        printf("Write Timed out.\n");
    }

    //Destroy the remote
    if(bev) {
    		bufferevent_free(bev);
    }
    if(ctx){
        free(ctx);
    }

}

static void thread_libevent_process(evutil_socket_t fd, short which, void *arg) {
    LIBEVENT_THREAD *me = (LIBEVENT_THREAD *)arg;
    CQ_ITEM *item;
    char buf[1];
    conn *c;
    unsigned int fd_from_pipe;

    if (read(fd, buf, 1) != 1) {
        fprintf(stderr, "Can't read from libevent pipe\n");
        return;
    }

    switch (buf[0]) {
    case 'c':{

        item = cq_pop(me->new_conn_queue);
        if (NULL != item) {
            struct bufferevent *bev;
            bev = bufferevent_socket_new(me->base, item->sfd, BEV_OPT_CLOSE_ON_FREE);
            if (!bev) {
                fprintf(stderr, "Error constructing bufferevent!");
                //event_base_loopbreak(me->base);
                return;
            }
            //unsigned int optval = 1;
            //setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));//禁用NAGLE算法

            //struct timeval tv_read = { 5*1000, 0 }, tv_write = { 60, 0 };
            //bufferevent_set_timeouts (bev, &tv_read, NULL);
            conn *client = (conn*)malloc(1* sizeof(conn));//client_new(bev, item->init_state);
            if(client == NULL){
                fprintf(stderr, "Can't listen for events on fd %d\n",item->sfd);
                bufferevent_free(bev);
                return;
            }
          	bufferevent_setcb(bev, readcb, writecb, errorcb, client);
          	//bufferevent_setwatermark(bev, EV_READ, 60, 0);	//读取数据至少60字节
    		bufferevent_enable(bev, item->event_flags);
            cqi_free(item);
            printf("New client: %s \n");

        }
        break;
    }

    }
    return;
}


static void setup_thread(LIBEVENT_THREAD *me) {

#if defined(LIBEVENT_VERSION_NUMBER) && LIBEVENT_VERSION_NUMBER >= 0x02000101
    struct event_config *ev_config;
    ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    me->base = event_base_new_with_config(ev_config);
    event_config_free(ev_config);
#else
    me->base = event_init();
#endif

    if (! me->base) {
        fprintf(stderr, "Can't allocate event base\n");
        exit(1);
    }

    /* Listen for notifications from other threads */
    event_set(&me->notify_event, me->notify_receive_fd,
              EV_READ | EV_PERSIST, thread_libevent_process, me);
    event_base_set(me->base, &me->notify_event);

    if (event_add(&me->notify_event, 0) == -1) {
        fprintf(stderr, "Can't monitor libevent notify pipe\n");
        exit(1);
    }

    me->new_conn_queue = (struct conn_queue*)malloc(sizeof(struct conn_queue));
    if (me->new_conn_queue == NULL) {
        perror("Failed to allocate memory for connection queue");
        exit(EXIT_FAILURE);
    }
    cq_init(me->new_conn_queue);





}


static void create_worker(void *(*func)(void *), void *arg) {
    pthread_attr_t  attr;
    int             ret;

    pthread_attr_init(&attr);

    if ((ret = pthread_create(&((LIBEVENT_THREAD*)arg)->thread_id, &attr, func, arg)) != 0) {
        fprintf(stderr, "Can't create thread: %s\n", strerror(ret));
        exit(1);
    }
}

static void register_thread_initialized(void) {
    pthread_mutex_lock(&init_lock);
    init_count++;
    pthread_cond_signal(&init_cond);
    pthread_mutex_unlock(&init_lock);

}

static void *worker_libevent(void *arg) {
    LIBEVENT_THREAD *me = (LIBEVENT_THREAD *)arg;


    register_thread_initialized();

    event_base_loop(me->base, 0);

    // same mechanism used to watch for all threads exiting.
    register_thread_initialized();

    event_base_free(me->base);
    return NULL;
}


static void wait_for_thread_registration(int nthreads) {
    while (init_count < nthreads) {
        pthread_cond_wait(&init_cond, &init_lock);
    }
}

void memcached_thread_init(int nthreads, void *arg) {
    int         i;

    pthread_mutex_init(&init_lock, NULL);
    pthread_cond_init(&init_cond, NULL);

    pthread_mutex_init(&cqi_freelist_lock, NULL);

    threads = (LIBEVENT_THREAD *)calloc(nthreads, sizeof(LIBEVENT_THREAD));
    if (! threads) {
        perror("Can't allocate thread descriptors");
        exit(1);
    }

    for (i = 0; i < nthreads; i++) {
        int fds[2];
        if (pipe(fds)) {
            perror("Can't create notify pipe");
            exit(1);
        }

        threads[i].notify_receive_fd = fds[0];
        threads[i].notify_send_fd = fds[1];

        setup_thread(&threads[i]);

    }

    /* Create threads after we've done all the libevent setup. */
    for (i = 0; i < nthreads; i++) {
        //每个线程创建一个loop
        create_worker(worker_libevent, &threads[i]);
    }

    /* Wait for all the threads to set themselves up before returning. */
    pthread_mutex_lock(&init_lock);
    wait_for_thread_registration(nthreads);
    pthread_mutex_unlock(&init_lock);
}



//==================================== MAIN ==================================//




int stop_main_loop = 0;

static struct event_base *main_base;



static int last_thread = -1;
static LIBEVENT_THREAD *select_thread_round_robin(void)
{
    int tid = (last_thread + 1) % 10;

    last_thread = tid;

    return threads + tid;
}

void dispatch_conn_new(int sfd,  int event_flags, void *ssl) {
    CQ_ITEM *item = cqi_new();
    char buf[1];
    if (item == NULL) {
        close(sfd);
        /* given that malloc failed this may also fail, but let's try */
        fprintf(stderr, "Failed to allocate memory for connection object\n");
        return;
    }

    LIBEVENT_THREAD *thread;

    thread = select_thread_round_robin();

    item->sfd = sfd;
    item->event_flags = event_flags;
    item->ssl = ssl;

    cq_push(thread->new_conn_queue, item);

    buf[0] = 'c';
    if (write(thread->notify_send_fd, buf, 1) != 1) {
        perror("Writing to thread notify pipe");
    }
}


static void drive_machine(conn *c) {
    bool stop = false;
    int sfd;
    socklen_t addrlen;
    struct sockaddr_storage addr;
    int res;
    const char *str;

    addrlen = sizeof(addr);

    sfd = accept(c->sfd, (struct sockaddr *)&addr, &addrlen);

    if (sfd == -1) {
        if (errno == ENOSYS) {
            // 	ENOSYS 函数没有实现
            return;
        }
        perror("accept()");

        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // EWOULDBLOCK/EAGAIN 再尝试一下
            return;
        } else if (errno == EMFILE) {
            // EMFILE 打开文件过多
            fprintf(stderr, "Too many open connections\n");
            //accept_new_conns(false);
        } else {
            perror("accept()");
        }
        return;
    }

    if (fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL) | O_NONBLOCK) < 0) {
        perror("setting O_NONBLOCK");
        close(sfd);
        return;
    }
    void *ssl_v = NULL;
#ifdef TLS
    SSL *ssl = NULL;
    if (c->ssl_enabled) {
        assert(IS_TCP(c->transport) && settings.ssl_enabled);

        if (settings.ssl_ctx == NULL) {
            if (settings.verbose) {
                fprintf(stderr, "SSL context is not initialized\n");
            }
            close(sfd);
            return;
        }
        SSL_LOCK();
        ssl = SSL_new(settings.ssl_ctx);
        SSL_UNLOCK();
        if (ssl == NULL) {
            if (settings.verbose) {
                fprintf(stderr, "Failed to created the SSL object\n");
            }
            close(sfd);
            return;
        }
        SSL_set_fd(ssl, sfd);
        int ret = SSL_accept(ssl);
        if (ret <= 0) {
            int err = SSL_get_error(ssl, ret);
            if (err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL) {
                if (settings.verbose) {
                    fprintf(stderr, "SSL connection failed with error code : %d : %s\n", err, strerror(errno));
                }
                SSL_free(ssl);
                close(sfd);
                return;
            }
        }
    }
    ssl_v = (void*) ssl;
#endif

    dispatch_conn_new(sfd,  EV_READ | EV_PERSIST, ssl_v);

    return;
}

void conn_release_items(conn *c) {

}

static void conn_cleanup(conn *c){

}


static void conn_close(conn *c) {
    /* delete the event, the socket and the conn */
    event_del(&c->event);
    fprintf(stderr, "<%d connection closed.\n", c->sfd);
    conn_cleanup(c);
#ifdef TLS
    if (c->ssl) {
        SSL_shutdown(c->ssl);
        SSL_free(c->ssl);
    }
#endif
    close(c->sfd);


    return;
}

void event_handler(const evutil_socket_t fd, const short which, void *arg) {
    conn *c;
    c = (conn *)arg;
    printf("%s called\n",__FUNCTION__);
    /* sanity */
    if (fd != c->sfd) {
        fprintf(stderr, "Catastrophic: event fd doesn't match conn fd!\n");
        conn_close(c);
        return;
    }

    drive_machine(c);

    /* wait for next event */
    return;
}




int server_socket(int port){
    int sfd;
    int flags;
    int error;
    sfd = socket(AF_INET, SOCK_STREAM, 0);//若成功则返回一个sockfd（套接字描述符）

    struct sockaddr_in server_sockaddr;//一般是储存地址和端口的。用于信息的显示及存储使用

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);


    if ((flags = fcntl(sfd, F_GETFL, 0)) < 0 ||
        fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("setting O_NONBLOCK");
        close(sfd);
        return -1;
    }
    flags = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (void *)&flags, sizeof(flags));
    setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));

    //error = setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&flags, sizeof(flags));
    //if (error != 0) perror("setsockopt");

    //error = setsockopt(sfd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
    //if (error != 0) perror("setsockopt");

    //error = setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags));
    //if (error != 0) perror("setsockopt");


    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);//将主机的无符号长整形数转换成网络字节顺序。　
    if(bind(sfd, (struct sockaddr* ) &server_sockaddr, sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }
    if(listen(sfd, -1) == -1)
    {
        perror("listen");
        exit(1);
    }
    return sfd;
}

int main(){
    int retval = 0;
    conn c;

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

    // 创建工作线程,每个线程独立的 event loop
    memcached_thread_init(10 , 0);

    c.sfd = server_socket(12345);

    event_set(&c.event, c.sfd, EV_READ | EV_PERSIST, event_handler, (void *)&c);
    event_base_set(main_base, &c.event);
    if (event_add(&c.event, 0) == -1) {
        perror("event_add");
        return -1;
    }

    while (!stop_main_loop) {
        if (event_base_loop(main_base, EVLOOP_ONCE) != 0) {
            retval = -1;
            break;
        }
    }
    conn_close(&c);
    event_base_free(main_base);
    return retval;
}
