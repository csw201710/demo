#include <windows.h>
#ifdef __cplusplus
extern "C"
{
#endif

	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"

	__declspec(dllexport) int luaopen_mylib(lua_State *L);
	__declspec(dllexport) int luaopen_ldll(lua_State *L);
#ifdef __cplusplus
}
#endif


static int version(lua_State *L) {
	lua_pushstring(L, "1.0.0");
	return 1;
}

static int show(lua_State *L) {
	puts("[dll] this is a test!");
	return 1;
}

static const luaL_Reg ldll_func[] = {
	{ "version", version },
	{ "show", show },
	{ NULL, NULL },
};

static int _lua_load(lua_State *L) {
	luaL_newlib(L, ldll_func);
	return 1;
}

int luaopen_ldll(lua_State *L) { // require("ldll")会调用到这里
	puts("luaopen_ldll");
	luaL_requiref(L, "ldll", _lua_load, 1);
	lua_pop(L, 1);  /* remove lib */
	return 1;
}



//方法1
int luaopen_mylib(lua_State *L) {
	printf("this is a loadlib\n");
	return 1;
}
















BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

