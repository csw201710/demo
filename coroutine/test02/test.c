#include <ucontext.h>
#include <stdio.h>

#define MAX_COUNT (1<<30)

static ucontext_t uc[3];
static int count = 0;

void ping();
void pong();

void ping(){
   char tmp[40960];
    while(count < MAX_COUNT){
        printf("ping %d\n", ++count);
        // yield to pong
        swapcontext(&uc[1], &uc[2]); // 保存当前context于uc[1],切换至uc[2]的context运行
    }
}

void pong(){
    char tmp[40960];
    while(count < MAX_COUNT){
        printf("pong %d\n", ++count);
        // yield to ping
        swapcontext(&uc[2], &uc[1]);// 保存当前context于uc[2],切换至uc[1]的context运行
    }
}

char st1[81920];
char st2[81920];

int main(int argc, char *argv[]){
   

    // initialize context
    getcontext(&uc[1]);
    getcontext(&uc[2]);

    uc[1].uc_link = &uc[0]; // 这个玩意表示uc[1]运行完成后，会跳至uc[0]指向的context继续运行
    uc[1].uc_stack.ss_sp = st1; // 设置新的堆栈
    uc[1].uc_stack.ss_size = sizeof st1;
    makecontext (&uc[1], ping, 0);

    uc[2].uc_link = &uc[0]; // 这个玩意表示uc[2]运行完成后，会跳至uc[0]指向的context继续运行
    uc[2].uc_stack.ss_sp = st2; // 设置新的堆栈
    uc[2].uc_stack.ss_size = sizeof st2;
    makecontext (&uc[2], pong, 0);

    // start ping-pong
    swapcontext(&uc[0], &uc[1]); // 将当前context信息保存至uc[0],跳转至uc[1]保存的context去执行
  // 这里我稍微要多说几句，因为我迷惑过，我曾经困惑的一点在于uc[0]，为什么uc[0]不需要设置堆栈的信息？因为swapcontext已经帮我们做好了一切，swapcontext函数会将当前点的信息保存在uc[0]中，当然我们没有设置的话，默认的堆栈一定是主堆栈啦

    return 0;
}

