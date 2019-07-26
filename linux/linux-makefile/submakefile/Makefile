#TOP ./Makefile
CC = gcc        #编译工具
FLAGS =         #编译规则
TAG = test      #目标文件

#顶层目录
TOPDIR = $(shell pwd)

#输出文件路径
OBJDIR = $(TOPDIR)/obj
#输出目标文件路径
BINDIR = $(TOPDIR)/bin
#源文件路径
SRCDIR = $(TOPDIR)/src

INC = -I./include           #指定头文件检索的路径

export CC TAG TOPDIR SUBDIR OBJDIR BINDIR INC   #导出全局变量

all: CHECK $(SRCDIR) $(TAG)    #make 目标

#检测并创建需要的目录
CHECK:
	mkdir -p $(OBJDIR) $(BINDIR) 

#去执行对应目录中的Makefile文件
$(SRCDIR) : echo
	$(MAKE) -C $@

$(TAG):        #生成目标文件，查找源文件目录下的所有.o文件编译生成目标到指定路径下
	$(CC) -o $(addprefix $(BINDIR)/,$(TAG)) $$(find ${SRCDIR} -name '*.o')



CLEANDIR:echo
	make -C $(SRCDIR) clean

.PHONY : clean echo
clean :CLEANDIR
	-rm $(BINDIR)/$(TAG) -f