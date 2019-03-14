#include <stdio.h>
#include <dlfcn.h>
#include <string.h>


//gcc -fPIC -shared main.cpp -o libhook.so -ldl
/*
	在linux操作系统的动态链接库加载过程中，动态链接器会读取
	LD_PRELOAD环境变量的值和默认配置文件/etc/ld.so.preload的文件内容，
	并将读取到的动态链接库进行预加载，即使程序不依赖这些动态链接库，
	LD_PRELOAD环境变量和/etc/ld.so.preload配置文件中指定的动态链接库依然会被装载,
	它们的优先级比LD_LIBRARY_PATH环境变量所定义的链接库查找路径的文件优先级要高，
	所以能够提前于用户调用的动态库载入。
*/
#ifdef __cplusplus

extern "C" {
#endif
int unlink(const char *pathname);
#ifdef __cplusplus
}
#endif
typedef int (*oldfuc)(const char *);
int unlink(const char *pathname){
	int ret = 0;
	void *handle;
	
	oldfuc cac_func;
	printf("call hook unlink file:%s\n",pathname);
	 handle = dlopen("/lib64/libc.so.6", RTLD_LAZY);
   
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	}
	cac_func = (oldfuc)dlsym(handle, "unlink");
	if(strstr(pathname,"/tmp/")){
		dlclose(handle);
		return -1;
	}
	ret = cac_func(pathname);
	dlclose(handle);
	
	return ret;
	 
	 
	 
}

