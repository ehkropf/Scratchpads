cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(bttop bttop.cpp)

add_custom_target(run
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/top/input/input01.txt | ${CMAKE_BINARY_DIR}/bttop
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/top/output/output01.txt
    DEPENDS bttop
    )
