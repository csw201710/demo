#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static const int CONTROL_LEN = CMSG_LEN( sizeof(int) );

void send_fd( int fd, int fd_to_send )
{
    struct iovec iov[1];
    struct msghdr msg;
    

    iov[0].iov_base = NULL;
    iov[0].iov_len = 0;
    msg.msg_name    = NULL;//用于存放目的进程的struct sockaddr_in 
    msg.msg_namelen = 0;
    msg.msg_iov     = iov;//指向一个struct iovec的数组，数组中的每个成员表示一个数据块
    msg.msg_iovlen = 1;

    cmsghdr cm;
    cm.cmsg_len = CONTROL_LEN;
    cm.cmsg_level = SOL_SOCKET;//初始的协议
    cm.cmsg_type = SCM_RIGHTS;//这个值表明了控制信息类型 SCM_RIGHTS 附属数据对象是一个文件描述符
    *(int *)CMSG_DATA( &cm ) = fd_to_send;
    msg.msg_control = &cm;
    msg.msg_controllen = CONTROL_LEN;

    sendmsg( fd, &msg, 0 );//sendmsg 最通用的I/O函数。实际上，可以用recvmsg代替read、readv、recv和redvfrom
}

int recv_fd( int fd )
{
    struct iovec iov[1];
    struct msghdr msg;
    
    iov[0].iov_base = NULL;
    iov[0].iov_len = 0;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov     = iov;
    msg.msg_iovlen = 1;

    cmsghdr cm;
    msg.msg_control = &cm;
    msg.msg_controllen = CONTROL_LEN;

    recvmsg( fd, &msg, 0 );

    int fd_to_read = *(int *)CMSG_DATA( &cm );
    return fd_to_read;
}

int main()
{
    int pipefd[2];
    int fd_to_pass = 0;

    int ret = socketpair( PF_UNIX, SOCK_DGRAM, 0, pipefd );
    assert( ret != -1 );

    pid_t pid = fork();
    assert( pid >= 0 );

    if ( pid == 0 )
    {
        close( pipefd[0] );
        fd_to_pass = open( "test.txt", O_RDWR, 0666 );
        printf("before send_fd\n");
        send_fd( pipefd[1], ( fd_to_pass > 0 ) ? fd_to_pass : 0 );
        printf("after send_fd\n");
  
        close( fd_to_pass );
        exit( 0 );
    }

    close( pipefd[1] );
    printf("before recv_fd\n");
    fd_to_pass = recv_fd( pipefd[0] );
    printf("after recv_fd\n");
    char buf[1024];
    memset( buf, '\0', 1024 );
    
    read( fd_to_pass, buf, 1024 );
    
    printf( "I got fd %d and data 【%s】\n", fd_to_pass, buf );
    close( fd_to_pass );
}


