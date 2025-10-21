cmake_minimum_required(VERSION 3.10)
project(kdyBackEnd)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)

set(MUDUO_LIBRARY muduo_base muduo_net muduo_http)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARTY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
link_libraries(${PROJECT_SOURCE_DIR}/lib)

# 添加你的可执行文件
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
