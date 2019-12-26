aux_source_directory (. SRC_LIST)

# add_library: 
# 	生成动态库或静态库(第1个参数指定库的名字；
#	第2个参数决定是动态还是静态，如果没有就默认静态；
#	第3个参数指定生成库的源文件)

add_library (testFunc_shared SHARED ${SRC_LIST})
add_library (testFunc_static STATIC ${SRC_LIST})

# set_target_properties: 设置输出的名称，还有其它功能，如设置库的版本号等等
set_target_properties (testFunc_shared PROPERTIES OUTPUT_NAME "testFunc")
set_target_properties (testFunc_static PROPERTIES OUTPUT_NAME "testFunc")

#LIBRARY_OUTPUT_PATH: 库文件的默认输出路径，这里设置为工程目录下的lib目录
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)