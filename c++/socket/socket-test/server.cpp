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

#define MAXSOCKET 20

int main(int arg, char *args[])
{
    //create server socket
    int listen_st = server_socket(6789);
    /*
     * 声明epoll_event结构体变量ev，变量ev用于注册事件，
     * 数组events用于回传需要处理的事件
     */
    struct epoll_event ev, events[100];
    //生成用于处理accept的epoll专用文件描述符
    int epfd = epoll_create(MAXSOCKET);
    //把socket设置成非阻塞方式
    setnonblock(listen_st);
    //设置需要放到epoll池里的文件描述符
    ev.data.fd = listen_st;
    //设置这个文件描述符需要epoll监控的事件
    /*
     * EPOLLIN代表文件描述符读事件
     *accept,recv都是读事件
     */
    ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    /*
     * 注册epoll事件
     * 函数epoll_ctl中&ev参数表示需要epoll监视的listen_st这个socket中的一些事件
     */
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_st, &ev);

    while (1)
    {
        /*
         * 等待epoll池中的socket发生事件，这里一般设置为阻塞的
         * events这个参数的类型是epoll_event类型的数组
         * 如果epoll池中的一个或者多个socket发生事件，
         * epoll_wait就会返回，参数events中存放了发生事件的socket和这个socket所发生的事件
         * 这里强调一点，epoll池存放的是一个个socket，不是一个个socket事件
         * 一个socket可能有多个事件，epoll_wait返回的是有消息的socket的数目
         * 如果epoll_wait返回事件数组后，下面的程序代码却没有处理当前socket发生的事件
         * 那么epoll_wait将不会再次阻塞，而是直接返回，参数events里面的就是刚才那个socket没有被处理的事件
         */
        int nfds = epoll_wait(epfd, events, MAXSOCKET, -1);
        if (nfds == -1)
        {
            printf("epoll_wait failed ! error message :%s \n", strerror(errno));
            break;
        }
        int i = 0;
        for (; i < nfds; i++)
        {
            if (events[i].data.fd < 0)
                continue;
            if (events[i].data.fd == listen_st)
            {
                //接收客户端socket
                int client_st = server_accept(listen_st);
                /*
                 * 监测到一个用户的socket连接到服务器listen_st绑定的端口
                 *
                 */
                if (client_st < 0)
                {
                    continue;
                }
                //设置客户端socket非阻塞
                setnonblock(client_st);
                //将客户端socket加入到epoll池中
                struct epoll_event client_ev;
                client_ev.data.fd = client_st;
                client_ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_st, &client_ev);
                /*
                 * 注释：当epoll池中listen_st这个服务器socket有消息的时候
                 * 只可能是来自客户端的连接消息
                 * recv,send使用的都是客户端的socket，不会向listen_st发送消息的
                 */
                continue;
            }
            //客户端有事件到达
            if (events[i].events & EPOLLIN)
            {
                //表示服务器这边的client_st接收到消息
                if (socket_recv(events[i].data.fd) < 0)
                {
                    close_socket(events[i].data.fd);
                    //接收数据出错或者客户端已经关闭
                    events[i].data.fd = -1;
                    /*这里continue是因为客户端socket已经被关闭了，
                     * 但是这个socket可能还有其他的事件，会继续执行其他的事件，
                     * 但是这个socket已经被设置成-1
                     * 所以后面的close_socket()函数都会报错
                     */
                    continue;
                }
                /*
                 * 此处不能continue，因为每个socket都可能有多个事件同时发送到服务器端
                 * 这也是下面语句用if而不是if-else的原因，
                 */

            }
            //客户端有事件到达
            if (events[i].events & EPOLLERR)
            {
                printf("EPOLLERR\n");
                //返回出错事件，关闭socket，清理epoll池，当关闭socket并且events[i].data.fd=-1,epoll会自动将该socket从池中清除
                close_socket(events[i].data.fd);
                events[i].data.fd = -1;
                continue;
            }
            //客户端有事件到达
            if (events[i].events & EPOLLHUP)
            {
                printf("EPOLLHUP\n");
                //返回挂起事件，关闭socket，清理epoll池
                close_socket(events[i].data.fd);
                events[i].data.fd = -1;
                continue;
            }
        }
    }
    //close epoll
    close(epfd);
    //close server socket
    close_socket(listen_st);
    return 0;
}
