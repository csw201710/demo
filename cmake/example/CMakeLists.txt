cmake_minimum_required (VERSION 2.6)
project (ABC)

add_subdirectory (lib_testFunc)
add_subdirectory (src)

option(MYDEBUG "enable debug compilation" OFF)
# 外部开关
#cmake .. -DMYDEBUG=ON



#aux_source_directory(. SRC_LIST)
#aux_source_directory (test_func SRC_LIST)
#aux_source_directory (test_func1 SRC_LIST1)
#set( SRC_LIST 
#	main.cpp
#	 func.cpp)
#该命令是用来向工程添加多个指定头文件的搜索路径，路径之间用空格分隔
#include_directories (test_func test_func1) 

#include_directories (include)
#link_directories (${PROJECT_SOURCE_DIR}/lib)
#aux_source_directory (src SRC_LIST)
#add_executable(main ${SRC_LIST})
#set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#add_compile_options(-std=c++11 -Wall -g) 


#target_link_libraries (main -ltestFunc )


#	EXECUTABLE_OUTPUT_PATH ：目标二进制可执行文件的存放位置
#	PROJECT_SOURCE_DIR：工程的根目录

#add_subdirectory (src)





