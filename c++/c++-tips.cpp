/*
(++i) + (++i)  = 6  问题解析:
	执行顺序
	int i = 1;
	++i ;//i = 2
	++i ;//i = 3
	i + i ; //输出6
*/
	int i = 1;
00351CFE  mov         dword ptr [i],1  
	int a = (++i) + (++i);
00351D05  mov         eax,dword ptr [i]  
	int a = (++i) + (++i);
00351D08  add         eax,1  
00351D0B  mov         dword ptr [i],eax  
00351D0E  mov         ecx,dword ptr [i]  
00351D11  add         ecx,1  
00351D14  mov         dword ptr [i],ecx  
00351D17  mov         edx,dword ptr [i]  
00351D1A  add         edx,dword ptr [i]  
00351D1D  mov         dword ptr [a],edx  
	printf("i=%d\n", a);
00351D20  mov         eax,dword ptr [a]  
00351D23  push        eax  
00351D24  push        offset string "i=%d\n" (035AE7Ch)  
00351D29  call        _printf (0341B40h)  
00351D2E  add         esp,8  
	return 0;




new char(10) 和 new char[10] 的区别
	{
		std::unique_ptr<char> a = std::make_unique<char>(10);// new char(10);delete _Ptr; //分配一个char ,并10来初始化值
	}
	{
		std::unique_ptr<char> b = std::unique_ptr<char>(new char[10]); //delete _Ptr;  maybe 错误???
	}
	{
		std::unique_ptr<char[]> a = std::make_unique<char[]>(10); // new char[10] ;delete [] char;
	}
	{
		std::unique_ptr<char[]> b = std::unique_ptr<char[]>(new char[10]); //new char[10] 分配一个大小为10的char数组
	}
