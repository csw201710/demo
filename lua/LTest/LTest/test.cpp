#include "test.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif



void testApi() {



}



//栈上的任何元素：正的索引指的是栈上的绝对位置（从1开始）；负的索引则指从栈顶开始的偏移量。
void testStack() {
	lua_State * L;
	L = luaL_newstate();
#if 1
	lua_pushnumber(L, 994);
	lua_pushstring(L, "hello,lua");

	printf("lua_isnumber = %d\n", lua_isnumber(L, 1));
	printf("lua_isstring = %d\n", lua_isstring(L, 1));

	printf("lua_isnumber = %d\n", lua_isnumber(L, 2));
	printf("lua_isstring = %d\n", lua_isstring(L, 2));
#endif


#if 1
	lua_settop(L, 0);
	lua_getglobal(L, "LuaFunc");

	int index = 1;
	printf("lua_isstring = %d\n", lua_isstring(L, index));
	printf("lua_isnumber = %d\n", lua_isnumber(L, index));
	printf("lua_iscfunction = %d\n", lua_iscfunction(L, index));
	printf("lua_isuserdata = %d\n", lua_isuserdata(L, index));
	printf("lua_isinteger = %d\n", lua_isinteger(L, index));
	printf("lua_isfunction = %d\n", lua_isfunction(L, index));


	//⑥操作栈调回结果
	lua_pcall(L, 0, 4, 0);
	printf("%s\n", lua_tostring(L, 1));
	printf("%s\n", lua_tostring(L, 2));
	printf("%s\n", lua_tostring(L, 3));
	printf("%s\n", lua_tostring(L, 4));
#endif


	lua_close(L);
}