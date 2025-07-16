# 设置所需的 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 定义项目名称和语言。我根据您的文件夹结构推断项目名称为 "test01"。
project(test01 CXX)

set (PROJECT_NAME test01)

# 打印状态信息，方便调试
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")


# 根据 c_cpp_properties.json 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # 推荐关闭编译器特定扩展，以保证代码可移植性

# 假设您的项目有一个名为 main.cpp 的主源文件。
# 如果您有多个源文件，请将它们全部列在这里。
# 例如: add_executable(test01 main.cpp utils.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)

message(STATUS "Creating executable: ${PROJECT_NAME} from main.cpp")

# 添加编译定义，这些定义来自于您的 c_cpp_properties.json 文件。
# UNICODE 和 _UNICODE 通常用于 Windows 项目。
# _DEBUG 通常只在调试(Debug)模式下定义。
target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
)

# 为 Debug 构建类型添加特定的定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
)

# 添加头文件搜索路径。
# 这对应于 c_cpp_properties.json 中的 "${workspaceFolder}/**"
# 将 src 目录添加进去, 这样在 main.cpp 中就可以直接 #include "other_header.h"
# 而不是 #include "src/other_header.h"
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/src"
)


message(STATUS "CMake configuration finished.")

