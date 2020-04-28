#include <stdio.h>

void ping();
void pong();

void ping(){
    char tmp[40960];
    printf("ping\n");
    pong();
}

void pong(){
    char tmp[40960];
    printf("pong\n");
    ping();
}

int main(int argc, char *argv[]){
    ping();
    return 0;
}


