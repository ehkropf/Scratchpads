cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(hmed hmed.cpp)

add_custom_target(run ${CMAKE_BINARY_DIR}/hmed ${CMAKE_SOURCE_DIR}/ex/hmed/input/input01.txt
    DEPENDS hmed
    )
