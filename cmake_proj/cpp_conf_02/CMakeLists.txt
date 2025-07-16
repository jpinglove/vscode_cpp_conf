cmake_minimum_required(VERSION 3.10)

project(mytest02 VERSION 0.1.0 LANGUAGES C CXX)

# --- Set Output Directories ---
# This places the final executable and DLLs into a 'bin' directory inside the build folder,
# which simplifies running and debugging.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#cmakedefine USE_MYMATH


include(CTest)
enable_testing()

# 提供一个选项是OFF或者ON，如果没有初始值被提供则默认使用OFF
option(USE_MYMATH "Use tutorial provided math implementation" ON)

INCLUDE_DIRECTORIES(inc)

# Explicitly define the source file for the executable to avoid compiling library sources into it.
set(PROGRAMME_SOURCES src/main.cpp)
set(PROGRAMME mytest02)

add_executable(${PROGRAMME} ${PROGRAMME_SOURCES})

# This must be active to build the testlib library.
if (USE_MYMATH)
    add_subdirectory(lib)
endif()

if (USE_MYMATH)
    target_link_libraries(${PROGRAMME} PRIVATE testlib) # 使用 PRIVATE 或 PUBLIC 根据实际情况调整
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
