cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(bttop bttop.cpp)

add_executable(btbfs btbfs.cpp)

add_executable(btswap btswap.cpp)

add_custom_target(run
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/swap/input/input03.txt | ${CMAKE_BINARY_DIR}/btswap
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/swap/output/output03.txt
    DEPENDS btswap
    )
