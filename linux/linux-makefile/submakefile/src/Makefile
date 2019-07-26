#./src/Makefile
SUBDIR = sub001 sub002   #这里就添加源文件的目录结构

all:$(SUBDIR) main

#进入底层目录的Makefile
$(SUBDIR):echo
	make -C $@

main: main.cpp
	gcc -c $<

.PHONY: echo

CLEANDIR:
	rm *.o -f
	@for dir in $$(echo $(SUBDIR)); do make -C $$dir clean ; done
	

.PHONY : clean
clean :CLEANDIR
