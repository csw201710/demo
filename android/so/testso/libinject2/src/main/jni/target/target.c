/*
 * target.c
 *
 *  Created on: 2015年6月26日
 *      Author: Administrator
 */

#include<stdio.h>
#include <unistd.h>

void writeToFile(const char* s){
    const char * fileName = "/sdcard/123.txt";
    FILE* fp = fopen(fileName , "ab+");
    fwrite(s,strlen(s),1,  fp);
    fclose(fp);


}
int main(int argc,char **argv)
{
	static i =0;
    char buf[123]={0};
	while(1){
		sleep(1);
#if 0
		sprintf(buf, "i am target program %d\n",i++);
        writeToFile(buf);
#else
        fprintf(stdout, "i am target program %d\n",i++);
        fflush(stdout);
#endif
	}
	return 0;
}
