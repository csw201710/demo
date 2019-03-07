./configure --enable-static --disable-shared --prefix=/usr

	[# /]$  c++    -I./bin/usr/local/include/  -Wl,-Bstatic -L./bin/usr/local/lib   -lcppunit -Wl,-Bdynamic  main.cpp -o main -ldl
		  /tmp/ccysXbXw.o: In function `main':
			main.cpp:(.text+0x29): undefined reference to `CppUnit::TestResult::TestResult(CppUnit::SynchronizedObject::SynchronizationObject*)'



	[# /]$ c++ main.cpp -o main -ldl  -I./bin/usr/local/include/  -Wl,-Bstatic -L./bin/usr/local/lib   -lcppunit -Wl,-Bdynamic
	[# /]$

