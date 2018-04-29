cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_library(graph SHARED graph.cpp)

add_executable(ex1 ex1.cpp)
target_link_libraries(ex1 graph)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/ex1
    DEPENDS ex1
    )
