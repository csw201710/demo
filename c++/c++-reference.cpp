void remove_reference(){
		int a[] = { 1,2,3 };
		decltype(*a) b1 = a[0];
		std::remove_reference<decltype(*a)>::type b2 = a[0];
		a[0] = 4;
		std::cout << b1 << std::endl; //输出4
		std::cout << b2 << std::endl; //输出1
}

