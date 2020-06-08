#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>//htons()函数头文件
#include <netinet/in.h>//inet_addr()头文件
#include <fcntl.h>
#include <sys/epoll.h>
#include "sub.h"
struct fds{//文件描述符结构体，用作传递给子线程的参数  
	int epollfd;  
	int sockfd;  
};  
void* worker(void* arg){
	printf("start recv...\n");
	fds *p = (fds*)arg;
	while(1){
		
		if(socket_recv(p->sockfd)){
			break;
		}
		printf("next recv...\n");
	}
	printf("end recv...\n");
	
}

int main(int arg,char *args[])
{
    //端口号
    int port=6789;
    //服务端IP地址
    char ipaddr[30]={0};
    strcpy(ipaddr,"127.0.0.1");
    //connect server
    int st=connect_server(ipaddr,port);
	fds ds;
	pthread_t thread;  
	ds.sockfd = st;
	pthread_create(&thread,NULL, worker,(void*)&ds);//调用工作者线程处理数据  
    //send message
    //发送消息--
    socket_send(st);
    //close socket
    close(st);
    return 0;
}
