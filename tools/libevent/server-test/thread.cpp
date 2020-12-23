#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "thread.h"
#include "Log.h"
#ifdef TLS
#include "openssl/ssl.h"

#endif

#define ITEMS_PER_ALLOC 64

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
    QUEUE online_head;
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





//=============================clientlist====================================//
static void init_thread_head(LIBEVENT_THREAD* me){
    QUEUE_INIT(&me->online_head);
}
static void init_conn_head(conn * conn){
    QUEUE_INIT(&conn->head);
}

static void insert_client(LIBEVENT_THREAD* me, conn* item){
    QUEUE_INIT(&item->head);
    QUEUE_INSERT_TAIL(&me->online_head, &item->head);
}

static void remove_client(conn* item){
    QUEUE_REMOVE(&item->head);
    QUEUE_INIT(&item->head);
}

void close_all_client(LIBEVENT_THREAD* me, void (*callback)(conn * conn) ){
    while (!QUEUE_EMPTY(&me->online_head)) {
		QUEUE *q = QUEUE_HEAD(&me->online_head);
        QUEUE_REMOVE(q);
		conn *tmp = QUEUE_DATA(q, struct conn, head);
        callback(tmp);

	}
}


//===============================CONN=========================================//

void conn_close(conn *c) {
    /* delete the event, the socket and the conn */
    if(c->event){
        event_free(c->event);
        c->event = 0;
    }

#ifdef TLS
    if (c->ssl) {
        SSL_shutdown((SSL*)c->ssl);
        SSL_free((SSL*)c->ssl);
        c->ssl = 0;
    }
#endif
    remove_client(c);

    if(c->ip[0] != '\0' ){
        INFO("close client: [%s:%d]", c->ip, c->port);
    }else{
        INFO("fd %d connection closed.", c->sfd);
    }

    if(c->bev) {
        //INFO("%s conn:[%p]",__func__, c);
        //bufferevent_disable(c->bev, EV_READ);
        bufferevent_free(c->bev);
        c->bev = 0;
    }else{
        close(c->sfd);
    }
    free(c);
    return;
}

conn* client_new(void* ctx, int fd, struct bufferevent *bev){
    LIBEVENT_THREAD * me = (LIBEVENT_THREAD*) ctx;

    conn* c = (conn*)calloc(1, sizeof(conn));
    c->sfd = fd;
    c->bev = bev;
    c->state = init_state;
    //INFO("%s conn:[%p]",__func__, c);
    return c;
}



conn* server_new(int port){
    int sfd;
    int flags;
    int error;
    conn* c = 0;


    sfd = socket(AF_INET, SOCK_STREAM, 0);//若成功则返回一个sockfd（套接字描述符）

    struct sockaddr_in server_sockaddr;//一般是储存地址和端口的。用于信息的显示及存储使用

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);


    if ((flags = fcntl(sfd, F_GETFL, 0)) < 0 ||
        fcntl(sfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("setting O_NONBLOCK");
        close(sfd);
        return 0;
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
    c = (conn*)calloc(1, sizeof(conn));
    c->sfd = sfd;
    c->state = init_state;

    init_conn_head(c);
    INFO("%s conn:[%d]",__func__, c->sfd);
    return c;
}





//===============================CQ===========================================//

static void cq_init(CQ *cq) {
    int ret;
    ret = pthread_mutex_init(&cq->lock, NULL);
    assert(ret == 0);
    cq->head = NULL;
    cq->tail = NULL;
}

static void cq_destory(CQ *cq){
    int ret;
    ret = pthread_mutex_destroy(&cq->lock);
    assert(ret == 0);
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

#include <set>
static 	std::set<void*> cq_malloc;
static void cq_malloc_add( void* arg){
    if(arg){
        cq_malloc.insert(arg);
    }

}

static void cq_malloc_free(){
    for (std::set<void*>::iterator it = cq_malloc.begin(); it != cq_malloc.end(); ++it){
        void* arg = *it;
        INFO("%s free(%p)", __func__,arg)
        free(arg);
    }
	cq_malloc.clear();

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

        //TODO 貌似这玩意没有精确释放了...
        /* Allocate a bunch of items at once to reduce fragmentation */
        item = (CQ_ITEM *)malloc(sizeof(CQ_ITEM) * ITEMS_PER_ALLOC);
        if (NULL == item) {
            return NULL;
        }
        cq_malloc_add(item);

        /*
         * Link together all the new items except the first one
         * (which we'll return to the caller) for placement on
         * the freelist.
         */
        for (i = 2; i < ITEMS_PER_ALLOC; i++)
            item[i - 1].next = &item[i];

        pthread_mutex_lock(&cqi_freelist_lock);
        item[ITEMS_PER_ALLOC - 1].next = cqi_freelist;
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



//=============================THREAD=========================================//



static void readcb(struct bufferevent *bev, void *ctx)
{
    conn* c = (conn*)ctx;
    char msg_header[1024] = {0};
    size_t msg_len;

    struct evbuffer *datain;
    struct evbuffer *dataout;

    const char* html = "HTTP/1.1 200 OK\r\n"
		"Server: Apache-Coyote/1.1\r\n"
		"Content-Type: text/html;charset=utf-8\r\n"
		"Connection: close\r\n"
		"Content-Length: %d\r\n"
		"\r\n%s";
    const char* body = "<html><head></head><body>abcdefghijk</body></html>";

    datain = bufferevent_get_input(bev);
	msg_len = evbuffer_get_length(datain);



    int n = evbuffer_copyout(datain, msg_header, sizeof(msg_header));
    //INFO("%s [%d] [%s]",__FUNCTION__, n, msg_header);

    if (strstr(msg_header, "\r\n\r\n") != 0){
        char buf[2048] = {0};
        dataout = bufferevent_get_output(bev);
        snprintf(buf, sizeof(buf) - 1, html, strlen(body), body);


        evbuffer_add(dataout, buf, strlen(buf));

        //evbuffer_drain 将数据从缓冲区前面移除
        evbuffer_drain(datain, msg_len);

        //conn_close(c);
        bufferevent_disable(bev, EV_READ);
        c->state = terminal_state;
    }





}
static void writecb(struct bufferevent *bev, void *ctx)
{
    INFO("%s called",__FUNCTION__);
    conn *c = (conn*) ctx;
    if(c->state == terminal_state){
        conn_close(c);
    }

}
static void errorcb(struct bufferevent *bev, short error, void *ctx)
{
    conn* c = (conn*) ctx;
    INFO("%s called",__FUNCTION__);

    if (error & BEV_EVENT_CONNECTED)
    {
    		ERROR("Get BEV_EVENT_CONNECTED event.");
    }
    else if (error & BEV_EVENT_EOF)
    {
        /* connection has been closed, do any clean up here */
        WARN("connection closed.");
    }
    else if (error & BEV_EVENT_ERROR)
    {
        /* check errno to see what error occurred */
        ERROR("errorcb some other error, %s.",  strerror(errno));
    }
    else if (error & (BEV_EVENT_TIMEOUT|BEV_EVENT_READING))
    {
        /* must be a timeout event handle, handle it */
        ERROR("Read Timed out.");
    }
    else if (error & (BEV_EVENT_TIMEOUT|BEV_EVENT_WRITING))
    {
        /* must be a timeout event handle, handle it */
        ERROR("Write Timed out.");
    }

    //Destroy the remote

    conn_close(c);
}



static void thread_libevent_process(evutil_socket_t fd, short which, void *arg) {
    LIBEVENT_THREAD *me = (LIBEVENT_THREAD *)arg;
    CQ_ITEM *item = 0;
    char buf[1];
    conn *c;
    //INFO("[%s] called",__func__);
    if (read(fd, buf, 1) != 1) {
        ERROR("Can't read from libevent pipe");
        return;
    }

    switch (buf[0]) {
    case 'c':{
        item = cq_pop(me->new_conn_queue);
        if (NULL != item) {
            struct bufferevent *bev;
            bev = bufferevent_socket_new(me->base, item->sfd, BEV_OPT_CLOSE_ON_FREE);
            if (!bev) {
                ERROR("Error constructing bufferevent!");
                //event_base_loopbreak(me->base);
                cqi_free(item);
                return;
            }
            //unsigned int optval = 1;
            //setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));//禁用NAGLE算法

            struct timeval tv_read = { 10, 0 }, tv_write = { 60, 0 };
            bufferevent_set_timeouts (bev, &tv_read, NULL);
            conn *client = client_new(me , item->sfd, bev);//client_new(bev, item->init_state);
            if(client == NULL){
                ERROR("Can't client_new on fd %d",item->sfd);
                bufferevent_free(bev);
                return;
            }
            // TODO 保存客户列表
            insert_client(me, client);

          	bufferevent_setcb(bev, readcb, writecb, errorcb, client);
          	//bufferevent_setwatermark(bev, EV_READ, 60, 0);	//读取数据至少60字节
    		bufferevent_enable(bev, item->event_flags);
            {

                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);

                //int client_fd = bufferevent_getfd(client->bev);
                getpeername(client->sfd, (struct sockaddr *)&client_addr, &client_addr_len);
                inet_ntop(AF_INET, &client_addr.sin_addr, client->ip, sizeof(client->ip));
                client->port = ntohs(client_addr.sin_port);
                INFO("[thread:0x%x process] New client: [%s:%d]",  me->thread_id,client->ip,client->port);
            }

            cqi_free(item);
        }
        break;
    }
    case 's':{
        //INFO("get s");
        event_base_loopexit(me->base, NULL);
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
        ERROR("Can't allocate event base");
        exit(1);
    }


    event_set(&me->notify_event, me->notify_receive_fd,
              EV_READ | EV_PERSIST, thread_libevent_process, me);
    event_base_set(me->base, &me->notify_event);

    if (event_add(&me->notify_event, 0) == -1) {
        ERROR("Can't monitor libevent notify pipe");
        exit(1);
    }

    me->new_conn_queue = (struct conn_queue*)malloc(sizeof(struct conn_queue));
    if (me->new_conn_queue == NULL) {
        ERROR("Failed to allocate memory for connection queue");
        exit(EXIT_FAILURE);
    }
    cq_init(me->new_conn_queue);
    init_thread_head(me);
    //INFO("[%s] called",__func__);
}



static int init_count = 0;
static pthread_mutex_t init_lock;
static pthread_cond_t init_cond;


static void create_worker(void *(*func)(void *), void *arg) {
    pthread_attr_t  attr;
    int             ret;

    pthread_attr_init(&attr);

    if ((ret = pthread_create(&((LIBEVENT_THREAD*)arg)->thread_id, &attr, func, arg)) != 0) {
        ERROR("Can't create thread: %s", strerror(ret));
        exit(1);
    }
    //INFO("[%s] called",__func__);
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
    event_base_free(me->base);
    me->base = 0;

    cq_destory(me->new_conn_queue);
    free(me->new_conn_queue);

    close_all_client(me, conn_close);

    // same mechanism used to watch for all threads exiting.
    register_thread_initialized();
    pthread_detach(pthread_self());
    return NULL;
}


static void wait_for_thread_registration(int nthreads) {
    while (init_count < nthreads) {
        pthread_cond_wait(&init_cond, &init_lock);
    }
}

static int total_threads = 0;

void memcached_thread_init(int nthreads, void *arg) {
    int         i;
    int ret;
    ret = pthread_mutex_init(&init_lock, NULL);
    assert(ret == 0);
    ret = pthread_cond_init(&init_cond, NULL);
    assert(ret == 0);
    ret = pthread_mutex_init(&cqi_freelist_lock, NULL);
    assert(ret == 0);

    cqi_freelist = NULL;

    total_threads = nthreads;
    threads = (LIBEVENT_THREAD *)calloc(total_threads, sizeof(LIBEVENT_THREAD));
    if (! threads) {
        perror("Can't allocate thread descriptors");
        exit(1);
    }

    for (i = 0; i < total_threads; i++) {
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
    for (i = 0; i < total_threads; i++) {
        //每个线程创建一个loop
        create_worker(worker_libevent, &threads[i]);
    }

    /* Wait for all the threads to set themselves up before returning. */
    init_count = 0;
    pthread_mutex_lock(&init_lock);
    wait_for_thread_registration(total_threads);
    pthread_mutex_unlock(&init_lock);
}




void stop_threads(void) {
    char buf[1];
    int i;
    int ret;
    INFO("asking background threads to stop");

    buf[0] = 's';
    ret = pthread_mutex_lock(&init_lock);
    assert(ret == 0);
    init_count = 0;
    for (i = 0; i < total_threads; i++) {
        if (write(threads[i].notify_send_fd, buf, 1) != 1) {
            perror("Failed writing to notify pipe");
            /* TODO: This is a fatal problem. Can it ever happen temporarily? */
        }
    }
    wait_for_thread_registration(total_threads);
    pthread_mutex_unlock(&init_lock);

    // 关闭pipe
    for(i = 0; i<total_threads;i++){
        close(threads[i].notify_send_fd);
        close(threads[i].notify_receive_fd);
    }

    //destory
    pthread_mutex_destroy(&cqi_freelist_lock);
    pthread_cond_destroy(&init_cond);
    pthread_mutex_destroy(&init_lock);

    //释放cqi_freelist
    cq_malloc_free();
    //释放线程组空间
    if(threads){
        free(threads);
        threads = 0;
    }
    INFO("all background threads stopped");
}



static int last_thread = -1;
static LIBEVENT_THREAD *select_thread_round_robin(void)
{
    int tid = (last_thread + 1) % total_threads;

    last_thread = tid;

    return threads + tid;
}





void dispatch_conn_new(int sfd,  int event_flags, void *ssl) {
    CQ_ITEM *item = cqi_new();
    char buf[1];
    if (item == NULL) {
        close(sfd);
        /* given that malloc failed this may also fail, but let's try */
        ERROR("Failed to allocate memory for connection object");
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
        ERROR("accept failed! errno:%d", errno);
        if (errno == ENOSYS) {
            // 	ENOSYS 函数没有实现
            ERROR("ENOSYS");
            return;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // EWOULDBLOCK/EAGAIN 再尝试一下
            return;
        } else if (errno == EMFILE) {
            // EMFILE 打开文件过多
            ERROR("Too many open connections");
            //accept_new_conns(false);
        } else {
            perror("accept()");
        }
        return;
    }

    if (fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL) | O_NONBLOCK) < 0) {
        ERROR("setting O_NONBLOCK");
        close(sfd);
        return;
    }
    void *ssl_v = NULL;
#ifdef TLS
    SSL *ssl = NULL;
    if (c->ssl_enabled) {
        //TODO SSL_CTX_new 需要释放  SSL_CTX_free (ssl_ctx);
        SSL_CTX *ssl_ctx = SSL_CTX_new (SSLv23_client_method());
        if(ssl_ctx == 0){
            ERROR("SSL_CTX_new failed");
            close(sfd);
            return;
        }
        ssl = SSL_new(ssl_ctx);
        if (ssl == NULL) {
            ERROR("Failed to created the SSL object");
            close(sfd);
            return;
        }
        SSL_set_fd(ssl, sfd);
        int ret = SSL_accept(ssl);
        if (ret <= 0) {
            int err = SSL_get_error(ssl, ret);
            if (err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL) {
                ERROR("SSL connection failed with error code : %d : %s", err, strerror(errno));
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




void event_handler(const evutil_socket_t fd, const short which, void *arg) {
    int server_sfd;
    conn* c;
    c = (conn*)arg;
    //INFO("%s called",__FUNCTION__);
    /* sanity */
    if (fd != c->sfd) {
        ERROR("Catastrophic: event fd doesn't match conn fd!");
        conn_close(c);
        return;
    }

    drive_machine(c);

    /* wait for next event */
    return;
}
