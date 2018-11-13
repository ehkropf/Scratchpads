cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "Build type defaulting to release")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-Wall -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(example example.cpp)
target_link_libraries(example ${ARMADILLO_LIBRARIES})

add_executable(example1 example1.cpp)
target_link_libraries(example1 ${ARMADILLO_LIBRARIES})

add_executable(order order.cpp)
target_link_libraries(order ${ARMADILLO_LIBRARIES})

add_executable(fftchk fftcheck.cpp)
target_link_libraries(fftchk ${ARMADILLO_LIBRARIES})
