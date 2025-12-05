cmake_minimum_required(VERSION 3.10)
project(kdyBackEnd)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# IF (CMAKE_BUILD_TYPE STREQUAL "Debug")        
#     ADD_DEFINITIONS(-DDEBUG)                   
# ENDIF()

set(MUDUO_LIBRARIES muduo_base muduo_net muduo_http)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(MySQL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARTY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
link_libraries(${PROJECT_SOURCE_DIR}/lib)

set(ALL_LIBRARIES ${MUDUO_LIBRARIES}
                ${MYSQL_LIBRARIES}
                mosquittopp)

# 添加你的可执行文件
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
