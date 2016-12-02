cmake_minimum_required(VERSION 3.7)

if(NOT UNITTEST_LIBRARY)
    find_library(UNITTEST_LIBRARY_DIR UnitTest++)
    if(NOT UNITTEST_LIBRARY_DIR)
        message(FATAL_ERROR "UnitTest++ library not found.")
    else()
        message(STATUS "Found UnitTest++ library: " ${UNITTEST_LIBRARY_DIR})
    endif()

    get_filename_component(UNITTEST_LIBRARY_DIR ${UNITTEST_LIBRARY_DIR} DIRECTORY)
    get_filename_component(UNITTEST_INCLUDE_DIR ${UNITTEST_LIBRARY_DIR} DIRECTORY)
    set(UNITTEST_INCLUDE_DIR "${UNITTEST_INCLUDE_DIR}/include")

    set(UNITTEST_LIBRARY UnitTest++)
endif()

include_directories(${UNITTEST_INCLUDE_DIR})
link_directories(${UNITTEST_LIBRARY_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
add_executable(sanity sanity.cpp)
target_link_libraries(sanity ${UNITTEST_LIBRARY})
