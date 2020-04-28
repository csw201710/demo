#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#if __APPLE__ && __MACH__
	#include <sys/ucontext.h>
#else 
	#include <ucontext.h>
#endif 

#define STACK_SIZE (1024*1024)
#define DEFAULT_COROUTINE 16

struct coroutine;

struct schedule {
	char stack[STACK_SIZE]; //有协程运行时的栈空间，新创建协程时在四大函数的getcontext之后，makecontext之前设置，协程运行时会以该空间作为栈空间
	ucontext_t main; //主协程，当创建的协程调用coroutine_yield切出控制权，实际上会切到主协程运行
	int nco; //当前正在运行的协程数量
	int cap; //协程指针容器的容量
	int running;//前正在运行的协程的id，如果在主协程运行，则为-1
	struct coroutine **co;//指针数组，每一个指针指向一个协程
};

struct coroutine {
	coroutine_func func;//协程运行时的工作函数
	void *ud; //工作函数的附属参数
	
	ucontext_t ctx; //系统结构:  协程的context
	
	struct schedule * sch; //该协程归属的schedule
	ptrdiff_t cap; //拷贝栈时候申请的内存的容量
	ptrdiff_t size; //拷贝栈后的有效数据大小
	int status; //协程状态
	char *stack; //拷贝栈数据申请的空间
};

struct coroutine * 
_co_new(struct schedule *S , coroutine_func func, void *ud) {
	struct coroutine * co = malloc(sizeof(*co));
	co->func = func;
	co->ud = ud;
	co->sch = S;
	co->cap = 0;
	co->size = 0;
	co->status = COROUTINE_READY;
	co->stack = NULL;
	return co;
}

void
_co_delete(struct coroutine *co) {
	free(co->stack);
	free(co);
}

struct schedule * 
coroutine_open(void) {
	struct schedule *S = malloc(sizeof(*S));
	S->nco = 0;
	S->cap = DEFAULT_COROUTINE;
	S->running = -1;
	S->co = malloc(sizeof(struct coroutine *) * S->cap);
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);
	return S;
}

void 
coroutine_close(struct schedule *S) {
	int i;
	for (i=0;i<S->cap;i++) {
		struct coroutine * co = S->co[i];
		if (co) {
			_co_delete(co);
		}
	}
	free(S->co);
	S->co = NULL;
	free(S);
}

int 
coroutine_new(struct schedule *S, coroutine_func func, void *ud) {
	struct coroutine *co = _co_new(S, func , ud);
	if (S->nco >= S->cap) {//判断当前上下文容量是否足够
		//添加新空间
		int id = S->cap;
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);
		S->co[S->cap] = co;
		S->cap *= 2;
		++S->nco;
		return id;
	} else {
		int i;
		for (i=0;i<S->cap;i++) {
			int id = (i+S->nco) % S->cap;
			if (S->co[id] == NULL) {
				S->co[id] = co;
				++S->nco;
				return id;
			}
		}
	}
	assert(0);
	return -1;
}

static void
mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
	struct schedule *S = (struct schedule *)ptr;
	int id = S->running;
	struct coroutine *C = S->co[id];
	C->func(S,C->ud);
	_co_delete(C);
	S->co[id] = NULL;
	--S->nco;
	S->running = -1;
}

void 
coroutine_resume(struct schedule * S, int id) {
	assert(S->running == -1);
	assert(id >=0 && id < S->cap);
	struct coroutine *C = S->co[id];
	if (C == NULL)
		return;
	int status = C->status;
	switch(status) {
	case COROUTINE_READY:
		getcontext(&C->ctx); //获取当前上下文, 初始化ucp结构体, 将当前上下文保存到ucp中
		
		C->ctx.uc_stack.ss_sp = S->stack; //公用栈地址
		C->ctx.uc_stack.ss_size = STACK_SIZE; //公用栈大小
		
		C->ctx.uc_link = &S->main; //指向后继上下文, 当前上下文运行终止时系统会恢复指向的上下文
		S->running = id; //指明当前运行的id
		C->status = COROUTINE_RUNNING;
		uintptr_t ptr = (uintptr_t)S;
		/*
		void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);
		unc是你你要执行的函数(你的任务)
		argc是这个函数的参数个数，而后面的可变部分，是你要传递给func的实参。
		*/
		makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));//makecontext创建一个新的上下文
		

		// 将当前context信息保存至S->main,跳转至C->ctx保存的context去执行
		// 因为swapcontext已经帮我们做好了一切，swapcontext函数会将当前点的信息保存在S->main中，当然我们没有设置的话，默认的堆栈一定是主堆栈啦
		swapcontext(&S->main, &C->ctx);//swapcontext会保存当前上下文并切换到另一个上下文
		break;
	case COROUTINE_SUSPEND:
		memcpy(S->stack + STACK_SIZE - C->size, C->stack, C->size);
		S->running = id;
		C->status = COROUTINE_RUNNING;
		swapcontext(&S->main, &C->ctx);
		break;
	default:
		assert(0);
	}
}


/*
高地址	----------------
        |    xxxx      |
		|  有效数据    |   stack_size
        |   dummy      |
低地址  |    xxxx      |
        ----------------   -> schedule->stack
*/
static void
_save_stack(struct coroutine *C, char *top) {
	//假定栈空间由高地址向低地址增长，拷贝有效数据到协程自己的buffer中保存
	char dummy = 0;
	assert(top - &dummy <= STACK_SIZE);
	if (C->cap < top - &dummy) {
		free(C->stack);
		C->cap = top-&dummy;
		C->stack = malloc(C->cap);
	}
	C->size = top - &dummy;
	memcpy(C->stack, &dummy, C->size);
}

void
coroutine_yield(struct schedule * S) {
	int id = S->running;
	assert(id >= 0);
	struct coroutine * C = S->co[id];
	assert((char *)&C > S->stack);
	_save_stack(C,S->stack + STACK_SIZE);
	C->status = COROUTINE_SUSPEND;
	S->running = -1;
	swapcontext(&C->ctx , &S->main);
}

int 
coroutine_status(struct schedule * S, int id) {
	assert(id>=0 && id < S->cap);
	if (S->co[id] == NULL) {
		return COROUTINE_DEAD;
	}
	return S->co[id]->status;
}

int 
coroutine_running(struct schedule * S) {
	return S->running;
}

