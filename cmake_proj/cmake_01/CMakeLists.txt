cmake_minimum_required(VERSION 3.10) # 指定CMake的最低版本要求

# project(cmake01 CXX) # 定义项目名称为cmake01，并指定语言为C++

# 定义项目名称、版本和语言。
project(cmake01 VERSION 1.0 LANGUAGES CXX)

set(SQRT_APP_NAME "sqrt_app")
set(BUBBLE_APP_NAME "bubble_sorter")

# 设置 C++ 标准为 C++17，并要求编译器必须支持。
# 同时推荐关闭编译器特定的扩展，以增强代码可移植性。
set(CMAKE_CXX_STANDARD 17) # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# 如果用户没有指定构建类型，则默认为 Debug。
# 这会自动为编译器添加 -g (GCC/Clang) 或 /Zi (MSVC) 等调试标志。
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
    message(STATUS "Build type not specified, defaulting to 'Debug'")
endif()


# =============================================================================
# Target 1: sqrt_app
# 计算平方根的应用
# =============================================================================

# 添加头文件目录
# 这样源文件就可以使用 #include "b.h" 而不是 #include "../inc/b.h"
# include_directories(inc)


# --- 创建第一个可执行文件：用于计算平方根 ---
# 将 main.cpp 和 b.cpp 编译链接成一个名为 sqrt_app 的可执行文件
# add_executable(sqrt_app src/main.cpp src/b.cpp)
add_executable(${SQRT_APP_NAME} src/main.cpp src/b.cpp)

target_include_directories(${SQRT_APP_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/inc"
)

# 因为 b.cpp 中使用了 sqrt 函数，我们需要链接数学库 (libm)
# target_link_libraries(sqrt_app PRIVATE m)
target_link_libraries(${SQRT_APP_NAME} PRIVATE m)


# --- 创建第二个可执行文件：用于冒泡排序 ---
# 将 bubble_sort.cpp 编译成一个名为 bubble_sorter 的可执行文件
add_executable(${BUBBLE_APP_NAME} src/bubble_sort.cpp)
