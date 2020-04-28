#include <stdio.h>
#include <ucontext.h> 
#include <unistd.h> 
int main(int argc, char *argv[]) 
{ 
  ucontext_t context; 
  getcontext(&context); 
  puts("Hello world"); 
  sleep(1); 
  setcontext(&context); 
  return 0; 
}

