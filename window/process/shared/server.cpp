#include<cstdio>
#include<cstdlib>
#include <iostream>
#include<Windows.h>

#define FileMapping_NAME "Xidian"
#define  FILESIZE 4096
LPVOID lpdata = NULL;//指针标识首地址

using namespace std;

int main()
{
    if (lpdata != NULL){
        cerr<<"Shared memory already exit!"<<endl;
    }

    //创建一个有名字标识的共享内存。
    HANDLE hmap =CreateFileMappingA(INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE | SEC_COMMIT,
            0,
            FILESIZE,
            FileMapping_NAME);

    if (hmap == NULL)  //如果句柄指向NULL，表示创建失败
    {
        cerr<<"Create shared memory failed"<<endl;
    }
    else{
        //映射文件到指针
        lpdata = MapViewOfFile(hmap,FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        if (lpdata == NULL)  //映射失败
        {
            cerr<<"Mapping failed!"<<endl;
        }
        else
        {
            char s[] = "Hello Xidian!";
            memcpy(lpdata, s, sizeof s);  //向这个内存中写入数据
        }
    }

    system("pause");

    UnmapViewOfFile(lpdata);//解除映射
    CloseHandle(hmap);

    system("pause");
    return 0;
}



