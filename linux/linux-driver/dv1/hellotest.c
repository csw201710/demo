#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    int val = 1;
    fd = open("/dev/hellodev", O_RDWR);
    if(fd < 0){
        printf("can't open!\n");
    }
    printf("%x\n",&val);
    write(fd, &val, 4);
    return 0;
}
