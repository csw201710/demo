#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


##########################################################################
# Macro: AC_PREREQ (version)
# 确保使用最新版本的Autoconf。 如果用于创建configure的Autoconf版本早于版本，则将错误消息输出到标准错误输出并退出并失败（退出状态为63）。
##########################################################################
AC_PREREQ([2.69])

###########################################################################
# Macro: AC_INIT (package, version, [bug-report], [tarname], [url])
# 设置包的名称及其版本
# 可选参数bug-report应该是用户应向其发送错误报告的电子邮件
# url应该是包的主页
###########################################################################
AC_INIT([helloworld], [1.0], [xxx@qq.com])


###########################################################################
# AM_INIT_AUTOMAKE([OPTIONS])
# 运行生成的Makefile正常运行所需的许多宏。
# 此宏也可以用另一种不推荐的形式调用：AM_INIT_AUTOMAKE（PACKAGE，VERSION，[NO-DEFINE]）。
# 在这种形式中，有两个必需的参数：包和版本号。这种用法大部分已经过时，因为可以从Autoconf的AC_INIT宏中获取包和版本。
###########################################################################
AM_INIT_AUTOMAKE

###########################################################################
# Macro: AC_CONFIG_SRCDIR (unique-file-in-source-dir)
# unique-file-in-source-dir是包源目录中的一些文件; 配置检查此文件的存在，以确保它所告知的目录实际上包含源代码。
# 偶尔有人用--srcdir意外指定了错误的目录; 这是一个安全检查。
###########################################################################
AC_CONFIG_SRCDIR([main.cpp])

###########################################################################
# AC_CONFIG_HEADERS (header ..., [cmds], [init-cmds])
# 该宏是实例化宏之一;
# example 1:
#			AC_CHECK_HEADERS([unistd.h windows.h])
#			这个宏将在当前建造环境下检查unistd.h,windows.h是否存在。并将两个参数写入到配置头文件中。一般是config.h，你可以使用AC_CONFIG_HEADERS([headers])来指定。
#				AC_CONFIG_HEADERS([config.h])
#			如果存在就会出现在config.h中例如下面:
#										/* Define to 1 if you have the <unistd.h> header file. */ 
#										#define HAVE_UNISTD_H 1 
#										/* Define to 1 if you have the <windows.h> header file. */ 
#										#define HAVE_WINDOWS_H 1
###########################################################################
AC_CONFIG_HEADERS([config.h])

###########################################################################
# AC_PROG_CXX ([compiler-search-list])
# 确定要使用的C ++编译器。 检查是否设置了环境变量CXX或CCC（按此顺序）; 
# 如果是，则将输出变量CXX设置为其值。
###########################################################################
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
###########################################################################
# Macro: AC_PROG_LIBTOOL
# Automake将打开libtool的处理（参见Libtool手册中的介绍）。
# 添加对--enable-shared， -  disable-shared， -  enable-static， -  disable-static， -  with-pic和--without-pic配置标志的支持
###########################################################################
AC_PROG_LIBTOOL

###########################################################################
# AC_CONFIG_MACRO_DIR (dir)
# 将dir指定为其他本地Autoconf宏的位置。 此宏旨在供将来版本的命令（如跟踪宏调用的autoreconf）使用。 它应该直接从configure.ac调用，以便为aclocal安装宏的工具可以找到宏的声明。
###########################################################################
AC_CONFIG_MACRO_DIR([m4])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

###########################################################################
# AC_CONFIG_FILES (file..., [cmds], [init-cmds])
# 包含要编译或安装的内容的发行版中的每个子目录都应附带一个文件Makefile.in，
# configure将在该文件中创建一个文件Makefile。 要创建Makefile，configure将执行一个简单的变量替换，
# 将Makefile.in中出现的'@ variable @'替换为configure为该变量确定的值。
###########################################################################
AC_CONFIG_FILES([Makefile])

###########################################################################
# AC_OUTPUT ([file]..., [extra-cmds], [init-cmds])
# 生成config.status并启动它。 在configure.ac结束时调用此宏一次。
# 这个宏是我们要输出的Makefile的名字
###########################################################################
AC_OUTPUT
