# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyProject CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件目录, 这样 #include "header.h" 才能找到 inc/header.h
include_directories(inc)

# 查找所有源文件
file(GLOB SOURCES "src/*.cpp")

# 添加可执行文件，并链接所有源文件
add_executable(my_app ${SOURCES})

