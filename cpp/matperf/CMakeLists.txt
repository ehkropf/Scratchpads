# Copyright 2017 Everett Kropf.
#

cmake_minimum_required(VERSION 3.5)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Defaulting to CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#####################################################################
set(SLIST buildmat.cpp)
add_executable(asdf ${SLIST})
target_link_libraries(asdf ${ARMADILLO_LIBRARIES})

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/asdf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS asdf
)
