﻿cmake_minimum_required(VERSION 3.6)


#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Release")
#    message(STATUS "Build type defaulting to 'Release'")
#endif()


#enable_testing()


set(CMAKE_EXPORT_COMPILE_COMMANDS true)


set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")


add_executable(pde explicit_heat.cpp)


#find_package(Armadillo REQUIRED)
#include_directories(${ARMADILLO_INCLUDE_DIRS})
#set(CMAKE_CXX_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")
#
#add_executable(trapode trapode.cpp)
#target_link_libraries(trapode ${ARMADILLO_LIBRARIES})
#
#set(TESTDIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
#add_test(NAME runtrap COMMAND trapode)
#add_test(NAME readdata COMMAND ${TESTDIR}/trapplot.py)
#
#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
#                  DEPENDS trapode)
