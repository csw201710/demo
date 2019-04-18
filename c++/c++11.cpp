/*
C++右值引用
  //int && abz2 = x + y; //abz2存放的是地址，使用时从该地址获取数据
	int && abz2 = x + y;
01114115  mov         eax,dword ptr [x]  
	int && abz2 = x + y;
01114118  add         eax,dword ptr [y]  
0111411B  mov         dword ptr [ebp-38h],eax  
0111411E  lea         ecx,[ebp-38h]  
01114121  mov         dword ptr [abz2],ecx  


	printf("r2 :%d\n", abz2);
0111413F  mov         eax,dword ptr [abz2]  
01114142  mov         ecx,dword ptr [eax]  
01114144  push        ecx  
01114145  push        offset string "r2 :%d\n" (01119B34h)  
0111414A  call        _printf (011114D8h)  
*/



