cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(balb balb.cpp)

add_custom_target(run ${CMAKE_BINARY_DIR}/balb ${CMAKE_SOURCE_DIR}/ex/balb/input/input04.txt > out
    COMMAND diff out ${CMAKE_SOURCE_DIR}/ex/balb/output/output04.txt
    DEPENDS balb
    )
