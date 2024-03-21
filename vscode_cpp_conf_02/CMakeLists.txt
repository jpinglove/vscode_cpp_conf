cmake_minimum_required(VERSION 3.0.0)
project(mytest02 VERSION 0.1.0 LANGUAGES C CXX)

#cmakedefine USE_MYMATH


include(CTest)
enable_testing()

# 提供一个选项是OFF或者ON，如果没有初始值被提供则默认使用OFF
option(USE_MYMATH "Use tutorial provided math implementation" ON)

INCLUDE_DIRECTORIES(inc)

aux_source_directory(src MY_CUSTOM_SRC_FILES_DIR)

SET(CUSTOM_VARIABLE_SOURCES_CODE_FILES
    ${MY_CUSTOM_SRC_FILES_DIR}
)
set(PROGRAMME mytest02)

add_executable(${PROGRAMME} ${CUSTOM_VARIABLE_SOURCES_CODE_FILES})

if (USE_MYMATH)
    # 添加一个名字为mathlib的子编译路径
    add_subdirectory(lib)
    list(APPEND EXTRA_LIBS testlib)

endif()


target_link_libraries(testlib PRIVATE )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
