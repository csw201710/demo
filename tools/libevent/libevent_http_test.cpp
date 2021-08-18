#include <stdio.h>
#include <iostream>

#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <vector>

#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
#include <event2/event.h>


#include <event2/util.h>
#include <event2/keyvalq_struct.h>

#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/util.h>
#include <event2/buffer_compat.h>

#include "event2/../http-internal.h"

#include <pthread.h>

#include <signal.h>

#include "event2/event.h"

#define LOG_ERROR std::cout

int openServer(const char* ip, int port) {
  	sockaddr_in in;
  	memset (&in, 0, sizeof (in));
  	in.sin_family = AF_INET;
  	in.sin_port = htons (port);
  	in.sin_addr.s_addr = inet_addr(ip);

  	evutil_socket_t fd = socket (AF_INET, SOCK_STREAM, 0);
  	if (fd == -1)
  	{
  		return -1;
  	}

  	if (evutil_make_socket_nonblocking (fd) < 0)
  	{
  		evutil_closesocket (fd);
  		return -1;
  	}

  	int on = 1;
  	if (setsockopt (fd, SOL_SOCKET, SO_KEEPALIVE, (const char *)&on, sizeof (on)) < 0)
  	{
  		evutil_closesocket (fd);
  		return -1;
  	}

  	if (evutil_make_listen_socket_reuseable (fd) < 0)
  	{
  		evutil_closesocket (fd);
  		return -1;
  	}

  	if (::bind (fd, (sockaddr*)&in, sizeof (sockaddr_in)) < 0)
  	{
  		evutil_closesocket (fd);
  		return -1;
  	}

  	listen (fd, 128);
  	return fd;
}



typedef struct {
  int fd;
  int count;
}Client;

extern "C" void httpd_handler(struct evhttp_request *req, void *arg) {
  char output[2048] = "\0";
  char tmp[1024];
  
  struct evbuffer *input = evhttp_request_get_input_buffer(req);
  int len = evbuffer_get_length(input);
  
  Client * client =(Client * ) arg;
  
  printf("pthread [%ld]\n", pthread_self());
  printf("evbuffer_get_length len = %d\n", len);
  //printf("evhttp_find_header Content-Length = %s\n", evhttp_find_header(evhttp_request_get_input_headers(req), "Content-Length"));
  //printf("User-Agent = %s\n", evhttp_find_header(evhttp_request_get_input_headers(req), "User-Agent"));
 
 
 
  //获取客户端请求的URI(使用evhttp_request_uri或直接req->uri)
  /*
  const char *uri;
  uri = evhttp_request_uri(req);
  sprintf(tmp, "uri=%s\n", uri);
  strcat(output, tmp);
  sprintf(tmp, "uri=%s\n", req->uri);
  strcat(output, tmp);
  // decoded uri
  char *decoded_uri;
  decoded_uri = evhttp_decode_uri(uri);
  sprintf(tmp, "decoded_uri=%s\n", decoded_uri);
  strcat(output, tmp);
  //解析URI的参数(即GET方法的参数)
  struct evkeyvalq params;
  //将URL数据封装成key-value格式,q=value1, s=value2
  evhttp_parse_query(decoded_uri, &params);
  //得到q所对应的value
  sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
  strcat(output, tmp);
  
  //
  
  free(decoded_uri);
  //获取POST方法的数据
  char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
  printf("post data = %s\n", post_data);
  printf("output = %s\n", output);
  */
  snprintf(output, sizeof(tmp)-1,"pthread [%ld]\n"
                                     "client.fd:[%d]\n" 
                                     "client.count:[%d]\n" 
                                     ,pthread_self()
                                     ,client->fd
                                     ,client->count++);
                                  
  /*
  具体的：可以根据GET/POST的参数执行相应操作，然后将结果输出
  ...
  */
  
  static bool i=true;
  if(i){
    //printf("sleep 5s\n");
    //sleep(5);
  }
  i = !i;
  
  
  /* 输出到客户端 */
  // HTTP header
  evhttp_add_header(req->output_headers, "Server", "This is Server");
  evhttp_add_header(req->output_headers, "Content-Type",
                    "text/plain; charset=UTF-8");
  evhttp_add_header(req->output_headers, "Connection", "keep-alive");
  evhttp_add_header(req->output_headers, "User-Agent", "curl");
  
#if 0
  //输出的内容
  struct evbuffer *buf;
  buf = evbuffer_new();
  evbuffer_add_printf(buf, "It works!\n%s\n", output);
  evhttp_send_reply(req, HTTP_OK, "OK", buf);
  evbuffer_free(buf);
#else
  struct evbuffer *out = evhttp_request_get_output_buffer(req);
  evbuffer_add(out, output, strlen(output));
  evhttp_send_reply(req, HTTP_OK, 0, 0);
  
#endif

}
 


static void* thread_run(void* arg) {
   int server_fd = *(int*)&arg;
   
    struct event_base * evbase_ = event_base_new();
    struct evhttp * httpd = evhttp_new(evbase_);
    evhttp_set_cb(httpd, "/sap2000server", httpd_handler, NULL);
    
    int ret = evhttp_accept_socket(httpd, server_fd);
    //  static evhttp_get_request
    if(ret != 0){
      event_base_loopexit (evbase_, 0);
      event_base_free (evbase_);
    }
    
    
    
    event_base_dispatch(evbase_);
}

int main2(int argc, char* argv[]){

  int server_fd = openServer("0.0.0.0", 8086);
  
  
  
    /*
    struct sockaddr_in cliaddr;  
    socklen_t len = sizeof(cliaddr);  
    int client_fd = accept(server_fd, (sockaddr *)&cliaddr, &len);
    if (-1 == client_fd) {  
        perror("accept fail");
    }
    char buff[INET_ADDRSTRLEN + 1] = {0};  
    inet_ntop(AF_INET, &cliaddr.sin_addr, buff, INET_ADDRSTRLEN);  
    uint16_t port = ntohs(cliaddr.sin_port);  
    printf("get a connection [%s] [%d]\n", buff, port);
    */
    //libevent http

    
   // break;
  pthread_t tpid[4096] ={0};
  int threadCount = 2;
  for(int i=0;i< threadCount;i++){
    
		 if ((pthread_create(&tpid[i], NULL, thread_run, (void*)server_fd)) == -1){
	 		 perror("create error!\n");
		 }else{
  			printf("create thread %d \n", i);
 		}
  }
  
	for(int i=0;i<threadCount;i++){
		if(tpid[i]){
			pthread_join(tpid[i], NULL);
		}
	}


  return 0;
}



static void
http_badreq_readcb(struct bufferevent *bev, void *arg)
{
		struct sockaddr_in client_addr;
		socklen_t client_addr_len;
		
  printf("[%s] call\n", __func__);
  struct evhttp * httpd = (struct evhttp *)arg;
  int fd = bufferevent_getfd(bev);
  getpeername(fd, (struct sockaddr *)&client_addr, &client_addr_len); 
  
    char buff[INET_ADDRSTRLEN + 1] = {0};  
    inet_ntop(AF_INET, &client_addr.sin_addr, buff, INET_ADDRSTRLEN);  
    uint16_t port = ntohs(client_addr.sin_port);  
    printf("get a connection [%s] [%d]\n", buff, port);
  
  //evhttp_get_request(httpd, fd, (struct sockaddr *)&client_addr, client_addr_len);
		
}

static void
http_writecb(struct bufferevent *bev, void *arg)
{
  printf("[%s] call\n", __func__);
	if (evbuffer_get_length(bufferevent_get_output(bev)) == 0) {
		/* enable reading of the reply */
		bufferevent_enable(bev, EV_READ);
	}
}



static void
http_badreq_errorcb(struct bufferevent *bev, short what, void *arg)
{
	printf("%s: called (what=0x%04x, arg=%p)\n", __func__, what, arg);
	/* ignore */
}


static void* thread_run_2(void* arg) {
   int client_fd = *(int*)&arg;
   struct bufferevent *bev = NULL;
   
   
    
   
   
    struct event_base * evbase_ = event_base_new();
    struct evhttp * httpd = evhttp_new(evbase_);
    
    Client * client = new Client;
    client->fd = client_fd;
    client->count = 0;
    evhttp_set_cb(httpd, "/sap2000server", httpd_handler, client);
    
        
        
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		
  printf("[%s] call [%d]\n", __func__, client_fd);
  
  
  getpeername(client_fd, (struct sockaddr *)&client_addr, &client_addr_len); 
  
    char buff[INET_ADDRSTRLEN + 1] = {0};  
    inet_ntop(AF_INET, &client_addr.sin_addr, buff, INET_ADDRSTRLEN);  
    uint16_t port = ntohs(client_addr.sin_port);  
    printf("get a connection [%s] [%d]\n", buff, port);
  
  evhttp_get_request(httpd, client_fd, (struct sockaddr *)&client_addr, client_addr_len);
        
        
        
        
    //bev = bufferevent_socket_new(evbase_, client_fd, BEV_OPT_CLOSE_ON_FREE);
	   //bufferevent_setcb(bev, http_badreq_readcb, http_writecb, http_badreq_errorcb, httpd);
    //bufferevent_enable(bev, EV_READ); //EV_WRITE | EV_PERSIST
    //bufferevent_setwatermark(bev, EV_READ, 1, 0);
    
    
    
      //event_base_loopexit (evbase_, 0);
      //event_base_free (evbase_);
    
    
    
    printf("starting...\n");
    event_base_dispatch(evbase_);
    printf("end...\n");
}


int main(int argc, char* argv[]){

  int server_fd = openServer("0.0.0.0", 8086);
  
  
  
  while(1){
    struct sockaddr_in cliaddr;  
    socklen_t len = sizeof(cliaddr);  
    pthread_t tpid;
    
    int client_fd = accept(server_fd, (sockaddr *)&cliaddr, &len);
    if (-1 == client_fd) {  
        perror("accept fail");
        sleep(2);
        continue;
    }
    printf("accept [%d]\n", client_fd);
    
    //libevent http
    if ((pthread_create(&tpid, NULL, thread_run_2, (void*)client_fd)) == -1){
     perror("create error!\n");
    }else{
    	printf("create thread  \n");
    }    

  }
    
  


  return 0;
}


