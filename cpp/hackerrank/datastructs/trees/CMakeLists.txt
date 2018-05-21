cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(bttop bttop.cpp)

add_executable(btbfs btbfs.cpp)

add_custom_target(run
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/bfs/input/input01.txt | ${CMAKE_BINARY_DIR}/btbfs
    #COMMAND cat ${CMAKE_SOURCE_DIR}/ex/bfs/output/output01.txt
    DEPENDS btbfs
    )
