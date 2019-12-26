aux_source_directory (. SRC_LIST)

# find testFunc.h
include_directories (../include)

link_directories (${PROJECT_SOURCE_DIR}/lib)

add_executable (main ${SRC_LIST})

if (MYDEBUG)
    message(STATUS "Currently is in debug mode")    
else()
    message(STATUS "Currently is not in debug mode")    
endif()

target_link_libraries (main testFunc )

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)