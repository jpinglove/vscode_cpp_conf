cmake_minimum_required(VERSION 3.0.0)
project(test04 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

INCLUDE_DIRECTORIES(inc)

aux_source_directory(src DIR_SRCS)

SET(SOURCES_CODE
    ${DIR_SRCS}
)


# add_executable(${PROJECT_NAME} ${TEST_HELLO})
add_executable(test04 ${SOURCES_CODE})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


