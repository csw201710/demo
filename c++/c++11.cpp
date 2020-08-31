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

/*
匿名函数
c++ 11(gcc 4.8.1以上)
	
C++中的匿名函数通常为[capture](parameters)->return-type{body}，当parameters为空的时候，()可以被省去，当body只有“return”或者返回为void，那么”->return-type“可以被省去
    capture：

    []        //未定义变量.试图在Lambda内使用任何外部变量都是错误的.
    [x, &y]   //x 按值捕获, y 按引用捕获.
    [&]       //用到的任何外部变量都隐式按引用捕获
    [=]       //用到的任何外部变量都隐式按值捕获
    [&, x]    //x显式地按值捕获. 其它变量按引用捕获
    [=, &z]   //z按引用捕获. 其它变量按值捕获


*/

