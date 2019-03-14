//start first line
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>

#ifdef _DEBUG
class WIN32_DEBUG_MEMORY_LEAK{
public:
	WIN32_DEBUG_MEMORY_LEAK(){
		//对 _CrtSetDbgFlag 的调用会导致在每个退出点自动调用 _CrtDumpMemoryLeaks
		::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void setBreakAlloc(int index){
		_CrtSetBreakAlloc(index); //在泄露出指定标识处，断下
	}
};
static WIN32_DEBUG_MEMORY_LEAK _dbg_leak;
#endif
//end



