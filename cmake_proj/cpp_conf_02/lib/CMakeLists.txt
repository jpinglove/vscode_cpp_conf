# Explicitly define the source files for the library.
# NOTE: The source file is in the parent src/ directory.
set(LIB_SOURCES ${CMAKE_SOURCE_DIR}/src/Utils.cpp)

# 添加库，可以是 SHARED (.dll) 或 STATIC (.lib/.a)
# 为了让导出宏生效，这里使用 SHARED
add_library(testlib SHARED ${LIB_SOURCES})

# 这是最关键的一步:
# 为 testlib 目标添加一个编译定义 TESTLIB_EXPORTS。
# 这会让头文件中的 UTILS_API 宏展开为 __declspec(dllexport)
target_compile_definitions(testlib PRIVATE TESTLIB_EXPORTS)

# 确保链接到此库的目标也能找到头文件
# PUBLIC 表示使用此库的消费者也需要包含这些目录
target_include_directories(testlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
