#ifndef _SERVER_H_

#define _SERVER_H_

#ifdef __cplusplus
extern "C"
{

#endif
//服务器创建socket
int server_socket(int port);

//设置非阻塞
int setnonblock(int st);

//接收客户端socket
int server_accept(int st);

//关闭socket
int close_socket(int st);

//接收消息
int socket_recv(int st);

//连接服务器
int connect_server(char *ipaddr,int port);

//发送消息
int socket_send(int st);

//将sockaddr_in转化成IP地址
int sockaddr_toa(const struct sockaddr_in * addr, char * ipaddr);

#ifdef __cplusplus
}
#endif

#endif

