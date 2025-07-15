cmake_minimum_required(VERSION 3.27.0)
project(makelib 
VERSION 0.1.0 
DESCRIPTION "The make a lib"
LANGUAGES C CXX)

# MAKE lib sample file codeing.

include(CTest)
enable_testing()
# the code add for custom
# target_include_directories(module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/module/include)

INCLUDE_DIRECTORIES(module/inc)
# target_include_directories(module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/module/inc)


# target_compile_definitions(module PRIVATE "EXPORT")
# target_compile_features(module PUBLIC cxx_std_20)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)

set(MAKE_LIB_PRJ
    ${DIR_SRCS}
)

# the code add for custom end

# 生成动态库 module.lib module.dll 用 SHARED
# 如果要生成静态库，将 SHARED 替换为 STATIC

# add_library(${PROJECT_NAME} SHARED module/src/makelib.cpp)

# add_executable(hello main.cpp)
ADD_EXECUTABLE(testlib src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


