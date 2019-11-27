#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*

// Structure used for storage of ancillary data object information.
struct cmsghdr
  {
    size_t cmsg_len;            // Length of data in cmsg_data plus length
                                //   of cmsghdr structure.
                                //   !! The type should be socklen_t but the
                                //   definition of the kernel is incompatible
                                //   with this.
    int cmsg_level;             // Originating protocol.
    int cmsg_type;              // Protocol specific type.
#if (!defined __STRICT_ANSI__ && __GNUC__ >= 2) || __STDC_VERSION__ >= 199901L
    __extension__ unsigned char __cmsg_data __flexarr; // Ancillary data.
#endif
  };

//{cmsg_len = 0, cmsg_level = 0, cmsg_type = 0, __cmsg_data = 0x7fffffffdfa0 ""}
*/

//计算cmsg_len的值
/*发送文件描述符 sv参数是用来传递信息的UNIX域socket，
*fd参数是待发送的文件描述符    */
int send_fd(int sv, int fd)
{
  struct msghdr msg;
  struct iovec iov[1];

  union {
      struct cmsghdr  cm;//sizeof 16
      char            space[CMSG_SPACE(sizeof(int))];//sizeof 24
      //char space[((((sizeof(int)) + sizeof (size_t) - 1) & (size_t) ~(sizeof (size_t) - 1)) + (((sizeof (struct cmsghdr)) + sizeof (size_t) - 1) & (size_t) ~(sizeof (size_t) - 1)))];
  } cmsg;

  if(fd == -1){
    msg.msg_control = 0;
    msg.msg_controllen = 0;
  }else{
    //将要传递的文件描述符赋值给cmsghdr的数据部分
    msg.msg_control = (caddr_t) &cmsg;
    msg.msg_controllen = sizeof(cmsg);
    memset(&cmsg, 0x00, sizeof(cmsg));
    cmsg.cm.cmsg_len = CMSG_LEN(sizeof(int));//用CMSG_LEN()宏获得的值
    cmsg.cm.cmsg_level = SOL_SOCKET;
    cmsg.cm.cmsg_type = SCM_RIGHTS;

    //memcpy(((&cmsg.cm)->__cmsg_data), &fd, sizeof(fd));
    //设置信息类型
    memcpy(CMSG_DATA(&cmsg.cm), &fd, sizeof(fd));
  }
  msg.msg_flags = 0;

  char buf[100]="QWERTYUIOPLKJHGFDSAZXCVBNM<>/";
  iov[0].iov_base = buf;
  iov[0].iov_len = sizeof(buf);
  //不需要传递其他数据，所以iov数据部分为空
  msg.msg_name = NULL;
  msg.msg_namelen = 0;
  //面向连接，可以不用
  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  int n = sendmsg(sv, &msg, 0);
  if (n == -1) {

      printf("sendmsg() failed \n");
      return -1;
  }
  return 0;
}

/*接收文件描述符
  *sv参数是用来传递信息的UNIX域socket */
int recv_fd(int sv)
{
  struct msghdr msg;
  struct iovec iov[1];
  int fd = 0;
  union {
      struct cmsghdr  cm;
      char            space[CMSG_SPACE(sizeof(int))];
  } cmsg;

  char buf[100];

  iov[0].iov_base = buf;
  iov[0].iov_len = sizeof(buf);


  msg.msg_name = NULL;
  msg.msg_namelen = 0;

  msg.msg_iov = iov;
  msg.msg_iovlen = 1;

  msg.msg_control = (caddr_t) &cmsg;//表示控制信息 指向一个cmsghdr结构体
  msg.msg_controllen = sizeof(cmsg); //msg_controllen表示控制信息的长度



  int n = recvmsg(sv, &msg, 0);
  if(n == -1){
    printf("recvmsg failed\n");
    return -1;
  }
  if(n == 0){
    printf("recvmsg() returned zero\n");
    return -1;
  }
  if(n < sizeof(buf)){
    printf("recvmsg() returned not enough data: %d\n",n);
    return -1;
  }
  printf("recv buf %s\n",buf);
  // memcpy(&fd, ((&cmsg.cm)->__cmsg_data), sizeof(int));
  memcpy(&fd, CMSG_DATA(&cmsg.cm), sizeof(int));//int fd = *(int*)CMSG_DATA(&cmsg.cm);

  if (msg.msg_flags & (MSG_TRUNC|MSG_CTRUNC)) {
      printf("recvmsg() truncated data\n");
  }
  //从cmsghdr的数据部分获取文件描述符

  return fd;
}

int main(int argc, char* argv[])
{
  int sv[2];
  int fd= 0;
  /*创建父子进程管道*/
  int ret = socketpair(PF_UNIX, SOCK_DGRAM, 0, sv);
  assert(ret != -1);
  //将创建的管道符存于sv[2]中
  printf("sv[0]: %d, sv[1]: %d\n",sv[0],sv[1]);
  pid_t pid = fork();
  if(pid == 0)//子进程
  {
      close(sv[0]);//关闭管道符一端
      fd = open("test.txt", O_RDWR);
      //  创建一个文件描述符
      send_fd(sv[1], (fd > 0) ? fd: -1);
      //在管道符另一端发送文件描述符
      //sleep(1);
      printf("child exit... sendfd : %d\n", fd);
      close(fd);
      //关闭文件
      exit(0);
  }

  //父进程
  close(sv[1]);//关闭管道符一端

  fd= recv_fd(sv[0]);
  if(fd > 0){
    char buf[1024];
    memset(buf, '\0', 1024);
    printf("read fd: %d \n", fd);

    read(fd, buf, 1024);
    printf("I got data %s\n",buf);
    close(fd);
  }


  return 0;
}


