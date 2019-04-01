#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>

pthread_mutex_t mutex;

void* another( void* arg )
{
    printf( "in child thread, lock the mutex\n" );
    pthread_mutex_lock( &mutex );
    sleep( 5 );
    pthread_mutex_unlock( &mutex );
}

void prepare()
{
    pthread_mutex_lock( &mutex );
}

void infork()
{
    pthread_mutex_unlock( &mutex );
}

int main()
{
    pthread_mutex_init( &mutex, NULL );
    pthread_t id;
    pthread_create( &id, NULL, another, NULL );
    
    /*
	    int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
				prepare	: 句柄将在fork调用创建出子进程之前被执行，它可以用来锁住所有父进程中的互斥锁。
				parent 	: 句柄则是fork调用创建出子进程之后，而fork返回之前，在父进程中被执行。它的作用是释放所有在prepare句柄中被锁住的互斥锁。
				child 	: 句柄是fork返回之前，在子进程中被执行。和parent句柄一样，child句柄也是用于释放所有在prepare句柄中被锁住的互斥锁。
    */
    //pthread_atfork( prepare, infork, infork );
    sleep( 1 );
    int pid = fork();
    if( pid < 0 )
    {
        pthread_join( id, NULL );
        pthread_mutex_destroy( &mutex );
        return 1;
    }
    else if( pid == 0 )
    {
        printf( "I anm in the child, want to get the lock\n" );
        pthread_mutex_lock( &mutex );
        printf( "I can not run to here, oop...\n" );
        pthread_mutex_unlock( &mutex );
        exit( 0 );
    }
    else
    {
        pthread_mutex_unlock( &mutex );
        wait( NULL );
    }
    pthread_join( id, NULL );
    pthread_mutex_destroy( &mutex );
    return 0;
}


