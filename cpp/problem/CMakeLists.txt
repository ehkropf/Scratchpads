cmake_minimum_required(VERSION 3.6)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

file(GLOB PROBLEM_HPP *.hpp)
add_executable(problem problem.cpp ${PROBLEM_HPP})

add_test(NAME run COMMAND problem)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS problem)
