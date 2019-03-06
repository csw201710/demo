#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int copy_file(const char* src,const char *dst)
{
    FILE *fp_src = NULL,*fp_des = NULL;
    char buf[1024];
    int num;
    
    if(access(src, F_OK) != 0){
    	ERROR("access f %s failed err:%d, %s",src,errno,strerror(errno));
    	return -1;
    }
    
    if(access(src, R_OK) != 0){
    	ERROR("access r %s failed err:%d, %s",src,errno,strerror(errno));
    	return -1;
    }
    
    if(access(dst, F_OK) == 0){
	    if(access(dst, W_OK) != 0){   
	        ERROR("access w %s failed err:%d, %s",dst,errno,strerror(errno));
	        return -1;
	    }   
    }
    
    if((fp_src=fopen(src,"rb"))==NULL)
    {
        ERROR("fopen src:%s err:%d, %s",src,errno,strerror(errno));
				return -1;
    }
    if((fp_des=fopen(dst,"wb"))==NULL)
    {
        ERROR("fopen dst:%s err:%d, %s [try again]",dst,errno,strerror(errno));
        unlink(dst);
    		if((fp_des=fopen(dst,"wb"))==NULL){
    			ERROR("fopen dst:%s err:%d, %s [try again failed]",dst,errno,strerror(errno));	
        	fclose(fp_src);
        	return -1;
    		}    
        
        

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
    return 1;
}

