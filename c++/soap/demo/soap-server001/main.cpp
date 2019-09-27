#include "calc.nsmap"

/*
//gsoap ns service name:	calc Simple calculator service described at https://www.genivia.com/dev.html
//gsoap ns service protocol:	SOAP
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://websrv.cs.fsu.edu/~engelen/calc.wsdl
//gsoap ns service location:	http://localhost:80

//gsoap ns schema namespace:	urn:calc

//gsoap ns service method: add Sums two values
int ns__add(double a, double b, double *result);

//gsoap ns service method: sub Subtracts two values
int ns__sub(double a, double b, double *result);

//gsoap ns service method: mul Multiplies two values
int ns__mul(double a, double b, double *result);

//gsoap ns service method: div Divides two values
int ns__div(double a, double b, double *result);

//gsoap ns service method: pow Raises a to b
int ns__pow(double a, double b, double *result);

soapcpp2.exe -w -x -L  -S -e  calc.h
	-w      don't generate WSDL and schema files
	-x      don't generate sample XML message files
	-e      generate SOAP RPC encoding style bindings
	-L      don't generate soapClientLib/soapServerLib
	-S      generate server-side code only
*/


SOAP_FMAC5 int SOAP_FMAC6 ns__add(struct soap*, double a, double b, double *result) {
	*result = a + b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__sub(struct soap*, double a, double b, double *result) {
	*result = a - b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__mul(struct soap*, double a, double b, double *result) {
	*result = a * b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__div(struct soap*, double a, double b, double *result) {
	*result = a / b;
	printf("%s\n", __FUNCTION__);
	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 ns__pow(struct soap*, double a, double b, double *result) {
	printf("%s\n", __FUNCTION__);
	return 0;
}
#ifdef USE_SIGNLE_THREAD
int main(int argc, char* argv[])
{

	int m, s; /* master and slave sockets */
	struct soap add_soap;
	soap_init(&add_soap);
	
	
	//soap_set_namespaces(&add_soap, add_namespaces);
	m = soap_bind(&add_soap, NULL, 8081, 100);
	if (m < 0)
	{
		soap_print_fault(&add_soap, stderr);
		exit(-1);
	}

	fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
	for (; ; )
	{
		s = soap_accept(&add_soap);
		if (s < 0)
		{
			soap_print_fault(&add_soap, stderr);
			exit(-1);
		}
		fprintf(stderr, "Socket connection successful: socket = %d\n", s);

		soap_serve(&add_soap);//该句说明该server的服务
		soap_end(&add_soap);
	}
	return 0;
}


#else



/////////////////////////////////////////////////////////////////////////
///宏与全局变量的定义
#define  BACKLOG (100)  
#define  MAX_THR (10)   
#define  MAX_QUEUE (1000)

#ifdef WIN32
#include <Windows.h>
#define pthread_t HANDLE
#define pthread_mutex_t  HANDLE 
#define pthread_cond_t  HANDLE 

#define pthread_mutex_lock(a) WaitForSingleObject(*a, INFINITE);
#define pthread_mutex_unlock(a) ReleaseMutex(*a)

#define pthread_cond_signal(a) SetEvent(*a)
#define pthread_cond_wait(a,b) WaitForSingleObject(*a, INFINITE);

#define pthread_mutex_init(a,b)  *a=CreateMutex(NULL,FALSE,NULL)

#define pthread_cond_init(a,b)  *a=CreateEvent(NULL,FALSE,FALSE,NULL);

#define pthread_mutex_destroy(a)  CloseHandle(*a)
#define pthread_cond_destroy(a) CloseHandle(*a)


#endif


pthread_mutex_t queue_cs;                        //队列锁
pthread_cond_t  queue_cv;                          //条件变量
SOAP_SOCKET     queue[MAX_QUEUE];   //数组队列
int                           head = 0, tail = 0;          //队列头队列尾初始化         
														   //////////////////////////////////////////////////////////////////////////


int         enqueue(SOAP_SOCKET);  //入队列函数
SOAP_SOCKET dequeue(void);         //出队列函数

								   //////////////////////////////////////////////////////////////////////////
								   //线程入口函数
#ifndef WIN32
void * process_queue(void * soap)
#else
DWORD WINAPI process_queue(LPVOID soap)
#endif
{
	struct soap * tsoap = (struct soap *)soap;
	for (;;)
	{
		tsoap->socket = dequeue();
		if (!soap_valid_socket(tsoap->socket))
		{
			break;
		}
		soap_serve(tsoap);
		soap_destroy(tsoap);
		soap_end(tsoap);
	}
	return NULL;
}

//入队列操作
int enqueue(SOAP_SOCKET sock)
{
	int status = SOAP_OK;
	int next;
	pthread_mutex_lock(&queue_cs);
	next = tail + 1;
	if (next >= MAX_QUEUE)
		next = 0;
	if (next == head)
		status = SOAP_EOM;
	else
	{
		queue[tail] = sock;
		tail = next;
	}
	pthread_cond_signal(&queue_cv);
	pthread_mutex_unlock(&queue_cs);
	return status;
}

//出队列操作
SOAP_SOCKET dequeue()
{
	SOAP_SOCKET sock;
#ifndef WIN32
	pthread_mutex_lock(&queue_cs);
	while (head == tail)
	{
		pthread_cond_wait(&queue_cv, &queue_cs);
	}
#else
	while (head == tail)
	{
		pthread_cond_wait(&queue_cv, &queue_cs);
	}
	pthread_mutex_lock(&queue_cs);

#endif
	sock = queue[head++];
	if (head >= MAX_QUEUE)
	{
		head = 0;
	}
	pthread_mutex_unlock(&queue_cs);
	return sock;
}

//主函数
int main(int argc, char ** argv)
{
	struct soap ServerSoap;
	//初始话运行时环境
	soap_init(&ServerSoap);

	{
		struct soap * soap_thr[MAX_THR];
		pthread_t tid[MAX_THR];
#ifdef WIN32
		DWORD dwThreadId[MAX_THR];
#endif

		int i, port = 8081;
		SOAP_SOCKET m, s;
		//锁和条件变量初始化
		pthread_mutex_init(&queue_cs, NULL);
		pthread_cond_init(&queue_cv, NULL);
		//绑定服务端口
		m = soap_bind(&ServerSoap, NULL, port, BACKLOG);
		//循环直至服务套接字合法
		while (!soap_valid_socket(m))
		{
			fprintf(stderr, "Bind port error! ");
			m = soap_bind(&ServerSoap, NULL, port, BACKLOG);
		}
		fprintf(stderr, "socket connection successful %d \n", m);

		//生成服务线程
		for (i = 0; i <MAX_THR; i++)

		{
			soap_thr[i] = soap_copy(&ServerSoap);
			fprintf(stderr, "Starting thread %d \n", i);
#ifndef WIN32
			pthread_create(&tid[i], NULL, (void*(*)(void*))process_queue, (void*)soap_thr[i]);
#else

			tid[i] = CreateThread(
				NULL,//default security attributes
				0,//use default stack size
				process_queue,//thread function
				(void*)soap_thr[i],//argument to thread function
				0,//use default creation flags
				&dwThreadId[i]);//returns the thread identifier
#endif
		}

		for (;;)
		{
			//接受客户端的连接
			s = soap_accept(&ServerSoap);
			if (!soap_valid_socket(s))
			{
				if (ServerSoap.errnum)
				{
					soap_print_fault(&ServerSoap, stderr);
					continue;
				}
				else
				{
					fprintf(stderr, "Server timed out ");
					break;
				}
			}
			//客户端的IP地址
			fprintf(stderr, "Accepted connection from IP= %d.%d.%d.%d socket = %d \n",
				((ServerSoap.ip) >> 24) && 0xFF, ((ServerSoap.ip) >> 16) & 0xFF, ((ServerSoap.ip) >> 8) & 0xFF, (ServerSoap.ip) & 0xFF, (ServerSoap.socket));
			//请求的套接字进入队列，如果队列已满则循环等待
			while (enqueue(s) == SOAP_EOM)
				Sleep(1000);
		}
		//服务结束后的清理工作
		for (i = 0; i < MAX_THR; i++)
		{
			while (enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
			{
				Sleep(1000);
			}
		}
#ifndef WIN32
		for (i = 0; i< MAX_THR; i++)
		{
			fprintf(stderr, "Waiting for thread %d to terminate ..", i);

			pthread_join(tid[i], NULL);
			fprintf(stderr, "terminated ");
			soap_done(soap_thr[i]);
			free(soap_thr[i]);
	    }
#else
		WaitForMultipleObjects(MAX_THR, tid, TRUE, INFINITE);
		for (i = 0;i<MAX_THR;i++)
		{
			CloseHandle(tid[i]);
			soap_done(soap_thr[i]);
			free(soap_thr[i]);
		}
#endif

		pthread_mutex_destroy(&queue_cs);
		pthread_cond_destroy(&queue_cv);
	}
	//分离运行时的环境
	soap_done(&ServerSoap);
	return 0;
}


#endif

