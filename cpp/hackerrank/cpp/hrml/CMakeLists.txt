cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(hrml hrml.cpp)

add_custom_target(run
    COMMAND cat ${CMAKE_SOURCE_DIR}/input/input00.txt | ${CMAKE_BINARY_DIR}/hrml
    DEPENDS hrml
    )
