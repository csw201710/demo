#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int copy_file(const char* src,const char *dst)
{
    FILE *fp_src,*fp_des;
    char buf[1024];
    int num;
    if((fp_src=fopen(src,"r"))==NULL)
    {
        perror("fopen src");
		return -1;
    }
    if((fp_des=fopen(dst,"w"))==NULL)
    {
        perror("fopen dst");
        return -1;
    }
    do
    {
        num=fread(buf,1,sizeof(buf),fp_src);
        fwrite(buf,1,num,fp_des);
        if(feof(fp_src) != 0) //如果文件结束，则返回非0值，否则返回0，文件结束符只能被clearerr()清除。
            break;
    }while(1);
    fclose(fp_src);
    fclose(fp_des);
}

