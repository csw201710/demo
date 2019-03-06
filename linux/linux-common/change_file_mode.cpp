#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int changeFileMode(const char * file_path,const char* filemode) 
{
    int mode;     //权限
    int mode_u;   //所有者user的权限
    int mode_g;   //所属组group的权限
    int mode_o;   //其他用户other的权限
    mode=atoi(filemode);
    if(mode>777||mode<0)
    {
        printf("mode num error!\n");
        return -1;
    }
   mode_u=mode/100;
	mode_g=mode/10%10;
    mode_o=mode%10;
    mode=(mode_u*8*8)+(mode_g*8)+mode_o;   //八进制转换

	if(chmod(file_path,mode)==-1) { 
		perror("chmod error");
		return -1;
	} 
	return 1;
}

int main(){
	changeFileMode("test.cpp", "755");
	return 0;
}

