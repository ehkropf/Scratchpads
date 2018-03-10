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

find_library(FMM_LIB hfmm2d)
if(NOT FMM_LIB)
    message(FATAL_ERROR "FMM library not found.")
else()
    message(STATUS "FMM library found: ${FMM_LIB}")
endif()

find_library(GFORTRAN_LIB gfortran)
if(NOT GFORTRAN_LIB)
    message(FATAL_ERROR "gFortran lib not found.")
else()
    message(STATUS "Found gFortran: ${GFORTRAN_LIB}")
endif()

set(MODSW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ModifiedSchwarz/src)
set(MODSW_LIBRARIES ${CMAKE_SOURCE_DIR}/ModifiedSchwarz/build/libschwarz.dylib)

#####################################################################
add_executable(bmat buildmat.cpp)
target_link_libraries(bmat ${ARMADILLO_LIBRARIES})

add_executable(eval eval.cpp)
target_include_directories(eval PUBLIC ${MODSW_INCLUDE_DIR})
target_link_libraries(eval ${ARMADILLO_LIBRARIES} ${MODSW_LIBRARIES})

add_executable(zfmm zfmm2dpart_call.cpp)
target_link_libraries(zfmm
    ${FMM_LIB}
    ${GFORTRAN_LIB}
    ${ARMADILLO_LIBRARIES}
)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/zfmm
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS zfmm
)
