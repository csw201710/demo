#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <errno.h>

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


#ifndef _WIN32
#include <dirent.h>
static int l_dir(lua_State *L) {
	DIR *dir;
	struct dirent *entry;
	int i;
	const char *path = luaL_checkstring(L, 1);
	/* open directory */
	dir = opendir(path);
	if (dir == NULL) { /* error opening the directory? */
		lua_pushnil(L); /* return nil and ... */
		lua_pushstring(L, strerror(errno)); /* error message */
		return 2; /* number of results */
	}
	/* create result table */
	lua_newtable(L);
	i = 1;
	while ((entry = readdir(dir)) != NULL) {
		lua_pushnumber(L, i++); /* push key */
		lua_pushstring(L, entry->d_name); /* push value */

		//做一个等价于 t[k] = v 的操作，这里 t 是给出的索引处的值，v 是栈顶的那个值，k 是栈顶之下的值。
		//这个函数会将键和值都弹出栈。跟在 Lua 中一样，这个函数可能触发一个 "newindex" 事件的元方法
		lua_settable(L, -3);
	}
	closedir(dir);
	return 1; /* table is already on top */
}
#endif

void error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

int fun1(lua_State *L)
{
	printf("function from cpp!\n");

	return 0;
}

int stdfile2(lua_State *L)
{
	printf("function stdfile2!\n");

	return 0;
}



// the io functions
static luaL_Reg const g_io_functions[] =
{
	{ "stdfile",            fun1 }
	,{ "stdfile2",              fun1 }
	,{ 0,              0 }
};

static luaL_Reg const mylib[] =
{
	{ "stdfile",            fun1 }
	,{ "stdfile2",              stdfile2 }
	,{ 0,              0 }
};


LUAMOD_API int luaopen_mylib(lua_State *L) {
	luaL_newlib(L, mylib);
	return 1;
}



/* call a function `f' defined in Lua */
static double f(lua_State *L, double x, double y) {
	double z = 0.0;
	/* push functions and arguments */
	lua_getglobal(L, "f"); /* function to be called */

	if (!lua_isfunction(L, -1))
		error(L, "`f' should be a function\n");

	lua_pushnumber(L, x); /* push 1st argument */
	lua_pushnumber(L, y); /* push 2nd argument */
						  /* do the call (2 arguments, 1 result) */
	if (lua_pcall(L, 2, 1, 0) != 0)
		error(L, "error running function `f': %s",
			lua_tostring(L, -1));
	/* retrieve result */
	if (!lua_isnumber(L, -1))
		error(L, "function `f' must return a number");
	z = lua_tonumber(L, -1);
	lua_pop(L, 1); /* pop returned value */
	return z;
}

static int getfield(lua_State *L, const char *key) {
	int result;
	lua_pushstring(L, key);
	//int lua_gettable(lua_State *L, int index);
	//把 t[k] 的值压栈，这里的 t 是指索引指向的值，而 k 则是栈顶放的值。
	lua_gettable(L, -2); /* get background[key] */
	if (!lua_isnumber(L, -1))
		error(L, "invalid component in background color");
	result = (int)lua_tonumber(L, -1) * 256;
	lua_pop(L, 1); /* remove number */
	return result;
}

static void stackDump(lua_State *L) {
	int i;
	int top = lua_gettop(L);
	printf("\n-------------------stack dump start------------------------\n");
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: /* strings */
			printf("`%s'", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN: /* booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER: /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		default: /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		printf(" "); /* put a separator */
	}
	printf("\n-------------------stack dump end------------------------\n");

}


void call_va(lua_State *L, const char *func, const char *sig, ...) {
	va_list vl;
	int narg, nres; /* number of arguments and results */
	va_start(vl, sig);
	//把全局变量 name 里的值压栈，返回该值的类型。
	lua_getglobal(L, func); /* get function */
							/* push arguments */
	narg = 0;
	while (*sig) { /* push arguments */
		switch (*sig++) {
		case 'd': /* double argument */
			lua_pushnumber(L, va_arg(vl, double));
			break;
		case 'i': /* int argument */
			lua_pushnumber(L, va_arg(vl, int));
			break;
		case 's': /* string argument */
			lua_pushstring(L, va_arg(vl, char *));
			break;
		case '>':
			goto endwhile;
		default:
			error(L, "invalid option (%c)", *(sig - 1));
		}
		narg++;
		luaL_checkstack(L, 1, "too many arguments");
	} endwhile:
	/* do the call */
	nres = strlen(sig); /* number of expected results */
	if (lua_pcall(L, narg, nres, 0) != 0) /* do the call */
		error(L, "error running function `%s': %s",
			func, lua_tostring(L, -1));
	/* retrieve results */
	nres = -nres; /* stack index of first result */
	while (*sig) { /* get results */
		switch (*sig++) {
		case 'd': /* double result */
			if (!lua_isnumber(L, nres))
				error(L, "wrong result type");
			*va_arg(vl, double *) = lua_tonumber(L, nres);
			break;
		case 'i': /* int result */
			if (!lua_isnumber(L, nres))
				error(L, "wrong result type");
			*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
			break;
		case 's': /* string result */
			if (!lua_isstring(L, nres))
				error(L, "wrong result type");
			*va_arg(vl, const char **) = lua_tostring(L, nres);
			break;
		default:
			error(L, "invalid option (%c)", *(sig - 1));
		}
		nres++;
	}
	va_end(vl);
}





void load(char *filename, int *width, int *height) {
	lua_State *L = luaL_newstate();
	luaopen_base(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run configuration file: %s",
			lua_tostring(L, -1));
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2))
		error(L, "`width' should be a number\n");
	if (!lua_isnumber(L, -1))
		error(L, "`height' should be a number\n");
	*width = (int)lua_tonumber(L, -2);
	*height = (int)lua_tonumber(L, -1);
	lua_close(L);
}

/*
C函数
*/
static int l_sin(lua_State *L) {
	double d = lua_tonumber(L, 1); /* get argument */
	lua_pushnumber(L, d/*sin(d)*/); /* push result */
	return 1; /* number of results */
}

#if 0
int l_map(lua_State *L) {
	int i, n;
	/* 1st argument must be a table (t) */
	luaL_checktype(L, 1, LUA_TTABLE);
	/* 2nd argument must be a function (f) */
	luaL_checktype(L, 2, LUA_TFUNCTION);
	n = luaL_getn(L, 1); /* get size of table */
	for (i = 1; i <= n; i++) {
		lua_pushvalue(L, 2); /* push f */
		lua_rawgeti(L, 1, i); /* push t[i] */
		lua_call(L, 1, 1); /* call f(t[i]) */
		lua_rawseti(L, 1, i); /* t[i] = result */
	}
	return 0; /* no results */
}
#endif

typedef struct NumArray {
	int size;
	double values[1]; /* variable part */
} NumArray;

static int newarray(lua_State *L) {
	int n = luaL_checkinteger(L, 1);
	size_t nbytes = sizeof(NumArray) + (n - 1) * sizeof(double);
	NumArray *a = (NumArray *)lua_newuserdata(L, nbytes);
	a->size = n;
	return 1; /* new userdatum is already on the stack */
}


static int setarray(lua_State *L) {
	NumArray *a = (NumArray *)lua_touserdata(L, 1);
	int index = luaL_checkinteger(L, 2);
	double value = luaL_checknumber(L, 3);
	luaL_argcheck(L, a != NULL, 1, "`array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->size, 2,
		"index out of range");
	a->values[index - 1] = value;
	return 0;
}


static int getarray(lua_State *L) {
	NumArray *a = (NumArray *)lua_touserdata(L, 1);
	int index = luaL_checkinteger(L, 2);
	luaL_argcheck(L, a != NULL, 1, "'array' expected");
	luaL_argcheck(L, 1 <= index && index <= a->size, 2,
		"index out of range");
	lua_pushnumber(L, a->values[index - 1]);
	return 1;
}

static int getsize(lua_State *L) {
	NumArray *a = (NumArray *)lua_touserdata(L, 1);
	luaL_argcheck(L, a != NULL, 1, "`array' expected");
	lua_pushnumber(L, a->size);
	return 1;
}

//static luaL_Reg const g_io_functions[] =
static luaL_Reg const  arraylib[] = {
	{ "new", newarray },
	{ "set", setarray },
	{ "get", getarray },
	{ "size", getsize },
	{ NULL, NULL }
};

LUAMOD_API int luaopen_arraylib(lua_State *L) {
	luaL_newlib(L, arraylib);
	return 1;
}
int luaopen_array(lua_State *L) {
	luaL_requiref(L, "array", luaopen_arraylib, 1);
	lua_pop(L, 1);  //从栈中弹出 1 个元素。
	//luaL_openlib(L, "array", arraylib, 0);
	return 1;
}

int main(int argc, char *argv[])
{

	lua_State * L;

	int sum = 0;
	//创建一个指向Lua解释器的指针。
	L = luaL_newstate();

#if 0
	lua_pushcfunction(L, fun1);
	lua_setglobal(L, "fun1"); //添加注册c的fun1函数
#else
	lua_register(L, "fun1", fun1);//注册C函数到lua
#endif
	//luaopen_mylib(L);	
#if 1
	luaL_requiref(L, "mylib", luaopen_mylib, 1);
	lua_pop(L, 1);  //从栈中弹出 1 个元素。
#else
	lua_pushstring(L, "mylib");
	lua_newtable(L);
	luaL_openlib(L, NULL, luaopen_mylib, 0);
	lua_settable(L, -3);
#endif
	
	lua_pushcfunction(L, l_sin);
	lua_setglobal(L, "mysin");

	luaopen_array(L);
	

	//函数加载Lua库
	luaL_openlibs(L);
	//加载脚本
	if (luaL_dofile(L, "add.lua") != 0) {
		printf("luaL_dofile failed\n");
	}


	printf("%.2f \n", f(L, -1, 1));

	const char* testfunc = "print(1,2)";//lua中调用c函数
	if (luaL_dostring(L, testfunc))    // 执行Lua命令。
		printf("Failed to invoke.\n");
#if 0
	const char * lua_buf = 
		" function add(x, y)\n "
		" fun1 \"lua\" \n"
		" return x + y \n"
		" end \n"
		" add(1,2) \n"
		""
		;

	if (luaL_dostring(L, lua_buf))    // 执行Lua命令。
		printf("Failed to invoke.\n");
	sum = (int)lua_tonumber(L, -1);
#else
	//函数名
	lua_getglobal(L, "add");

	//第一个参数压栈
	lua_pushnumber(L, 10);
	//返回栈顶元素的索引。 因为索引是从 1 开始编号的，所以这个结果等于栈上的元素个数；特别指出，0 表示栈为空。
	//printf("lua_gettop: %d\n", lua_gettop(L));
	//第二个参数压栈
	lua_pushnumber(L, 20);


	stackDump(L);
	//printf("lua_gettop: %d\n", lua_gettop(L));

	#if 1
	lua_pcall(L, 2, 1, 0);
	#else
	lua_call(L, 2, 1);
	#endif
	//得到返回值
	sum = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
#endif
	

	// print the result
	printf("The sum is %d\n", sum);
	//关闭 释放资源
	lua_close(L);

	return 0;
}