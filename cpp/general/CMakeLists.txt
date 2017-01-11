cmake_minimum_required(VERSION 3.6)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
add_executable(uptr uptr.cpp)
add_executable(shdat shdat.cpp)

###########################################################################
add_test(NAME uptr COMMAND uptr)
add_test(NAME shdat COMMAND shdat)
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS uptr shdat)
