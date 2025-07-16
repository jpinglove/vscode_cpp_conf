# CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(cmake02 CXX)

set(CMAKE_APP_NAME "cmake02")


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Build type not specified, defaulting to 'Debug'")
endif()

# 1. 使用 add_library 创建一个名为 'mylibrary' 的静态库(STATIC)。
#    它的源文件是 src/MyLibrary.cpp。
add_library(mylibrary STATIC src/MyLibrary.cpp)
# 2. 为 mylibrary 库指定公开的(PUBLIC)头文件目录。
#    这意味着任何链接到 mylibrary 的目标，都会自动获得这个头文件搜索路径。
target_include_directories(mylibrary PUBLIC
    "${PROJECT_SOURCE_DIR}/inc"
)
# 3. 为 mymath 库指定公开的(PUBLIC)链接依赖。
#    因为 mymath 使用了 'sqrt' 函数，它需要链接到数学库 'm'。
#    'PUBLIC' 关键字确保了任何链接到 mymath 的目标也会自动链接到 'm'。
target_link_libraries(mylibrary PUBLIC m)

# =============================================================================
# 添加头文件目录, 这样 #include "header.h" 才能找到 inc/header.h
include_directories(inc)

# 查找所有源文件
file(GLOB SOURCES "src/*.cpp")

# 添加可执行文件，并链接所有源文件
add_executable(${CMAKE_APP_NAME} ${SOURCES})

