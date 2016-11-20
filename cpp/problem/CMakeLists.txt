cmake_minimum_required(VERSION 3.6)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

###########################################################################
file(GLOB PROBLEM_HPP *.hpp)
add_library(problem SHARED ProblemLib.cpp ${PROBLEM_HPP})

###########################################################################
add_executable(example example.cpp)
target_link_libraries(example problem)

###########################################################################
add_test(NAME run COMMAND example)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS example)
