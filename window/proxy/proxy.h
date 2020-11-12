#ifndef __PROXY_H_00A__
#define __PROXY_H_00A__


/*
	ip: 代理ip
	port: 代理port
	type :  0 DIRECT
	        1 系统代理
			2 PAC代理
*/
int setProxy(const char* ip, int port, int type);

/*
type :  0 DIRECT
		1 系统代理
		2 PAC代理
*/
int resetProxy(int type);




#endif //__PROXY_H_00A__