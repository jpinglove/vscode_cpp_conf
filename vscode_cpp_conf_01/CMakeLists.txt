cmake_minimum_required(VERSION 3.0.0)

project(vscode_cpp_conf_01 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
# enable_testing()

INCLUDE_DIRECTORIES(inc)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

SET(VSCODE_CPP_CONF_01
    ${DIR_SRCS}
)
# add_executable(hello main.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${VSCODE_CPP_CONF_01})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

