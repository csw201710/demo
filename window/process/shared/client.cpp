#include <cstdio>
#include <cstdlib>
#include <Windows.h>
#include <iostream>

#define FileMapping_NAME "Xidian"
LPVOID lpdata = NULL;

using namespace std;
int main()
{
    //打开一个指定的文件映射对象，获得共享内存对象的句柄
    HANDLE hmapfile =OpenFileMappingA(FILE_MAP_READ, FALSE, FileMapping_NAME);
    if (hmapfile == NULL){
       cerr<<"Open mapfile failed"<<endl;//打开文件映射对象失败
    }
    else
    {
       //将一个文件映射对象映射到当前应用程序的地址空间。
       LPVOID lpbase = MapViewOfFile(hmapfile,FILE_MAP_READ, 0, 0, 0);
       if (lpbase == NULL)
       {
            cerr<<"Mapping failed!"<<endl;
       }
       else
       {
           char *p = (char *)lpbase;
           cout<<p<<endl;
       }
       UnmapViewOfFile(lpbase);//解除映射
       CloseHandle(hmapfile);//一个指定的文件映射对象

       system("pause");
    }
    return 0;
}


