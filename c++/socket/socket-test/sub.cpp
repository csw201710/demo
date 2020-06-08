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
#include "sub.h"

#define MAXBUF 1024

//创建socket
int socket_create()
{
    int st = socket(AF_INET, SOCK_STREAM, 0);
    if (st == -1)
    {
        printf("create socket failed ! error message :%s\n", strerror(errno));
        return -1;
    }
    return st;
}

//设置服务端socket地址重用
int socket_reuseaddr(int st)
{
    int on = 1;
    if (setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        printf("setsockopt reuseaddr failed ! error message :%s\n",
                strerror(errno));
        //close socket
        close_socket(st);
        return -1;
    }
    return 0;
}

//服务器绑定--监听端口号
int socket_bind(int st, int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    //type
    addr.sin_family = AF_INET;
    //port
    addr.sin_port = htons(port);
    //ip
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind ip address
    if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)
    {
        printf("bind failed ! error message :%s\n", strerror(errno));
        //close socket
        close_socket(st);
        return -1;
    }
    //listen
    if (listen(st, 20) == -1)
    {
        printf("listen failed ! error message :%s\n", strerror(errno));
        //close socket
        close_socket(st);
        return -1;
    }
    return 0;
}

//服务器创建socket
int server_socket(int port)
{
    if (port < 0)
    {
        printf("function server_socket param not correct !\n");
        return -1;
    }
    //create socket
    int st = socket_create();
    if (st < 0)
    {
        return -1;
    }
    //reuseaddr
    if (socket_reuseaddr(st) < 0)
    {
        return -1;
    }
    //bind and listen
    if (socket_bind(st, port) < 0)
    {
        return -1;
    }
    return st;
}

//连接服务器
int connect_server(char *ipaddr,int port)
{
    if(port<0||ipaddr==NULL)
    {
        printf("function connect_server param not correct !\n");
        return -1;
    }
    int st=socket_create();
    if(st<0)
    {
        return -1;
    }
    //conect server
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ipaddr);
    if(connect(st,(struct sockaddr *)&addr,sizeof(addr))==-1)
    {
        printf("connect failed ! error message :%s\n",strerror(errno));
        return -1;
    }
    return st;
}

//设置非阻塞
int setnonblock(int st)
{
    if (st < 0)
    {
        printf("function setnonblock param not correct !\n");
        //close socket
        close_socket(st);
        return -1;
    }
    int opts = fcntl(st, F_GETFL);
    if (opts < 0)
    {
        printf("func fcntl failed ! error message :%s\n", strerror(errno));
        return -1;
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(st, F_SETFL, opts) < 0)
    {
        printf("func fcntl failed ! error message :%s\n", strerror(errno));
        return -1;
    }
    return opts;
}

//接收客户端socket
int server_accept(int st)
{
    if (st < 0)
    {
        printf("function accept_clientsocket param not correct !\n");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    int client_st = accept(st, (struct sockaddr *) &addr, &len);
    if (client_st < 0)
    {
        printf("accept client failed ! error message :%s\n", strerror(errno));
        return -1;
    } else
    {
        char ipaddr[20] = { 0 };
        sockaddr_toa(&addr, ipaddr);
        printf("accept by %s\n", ipaddr);
    }
    return client_st;
}

//关闭socket
int close_socket(int st)
{
    if (st < 0)
    {
        printf("function close_socket param not correct !\n");
        return -1;
    }
    close(st);
    return 0;
}

//将sockaddr_in转化成IP地址
int sockaddr_toa(const struct sockaddr_in * addr, char * ipaddr)
{
    if (addr == NULL || ipaddr == NULL)
    {
        return -1;
    }
    unsigned char *p = (unsigned char *) &(addr->sin_addr.s_addr);
    sprintf(ipaddr, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
    return 0;
}

//接收消息
int socket_recv(int st)
{
    if (st < 0)
    {
        printf("function socket_recv param not correct !\n");
        return -1;
    }
    char buf[MAXBUF] = { 0 };
    int rc=0;
    rc=recv(st,buf,sizeof(buf),0);
    if(rc==0)
    {
        printf("client is close ! \n");
        return -1;
    }else if(rc<0)
    {
        /*
         * recv错误信息：Connection reset by peer
         * 错误原因：服务端给客户端发送数据，但是客户端没有接收，直接关闭，那么就会报错
         * 如果客户端接受了数据，再关闭，也不会报错，rc==0.
         */
        printf("recv failed ! error message :%s \n",strerror(errno));
        return -1;
    }
    printf("%s",buf);
    //send message
    /*
    memset(buf,0,sizeof(buf));
    strcpy(buf,"i am server , i have recved !\n");
    if(send(st,buf,strlen(buf),0)<0)
    {
        printf("send failed ! error message :%s \n",strerror(errno));
        return -1;
    }
    */
    return 0;
}

//发送消息
int socket_send(int st)
{
    char buf[MAXBUF]={0};
    while(1)
    {
        //read from keyboard
        read(STDIN_FILENO,buf,sizeof(buf));
        if(buf[0]=='0')
        {
            break;
        }
        if(send(st,buf,strlen(buf),0)<0)
        {
            printf("send failed ! error message :%s \n",strerror(errno));
            return -1;
        }
        memset(buf,0,sizeof(buf));
    }
    return 0;
}
