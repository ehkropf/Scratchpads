cmake_minimum_required(VERSION 3.6)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
add_executable(uptr uptr.cpp)
add_executable(shdat shdat.cpp)
add_executable(cont containers.cpp)
add_executable(cin cin.cpp)
add_executable(special special.cpp)

###########################################################################
#add_test(NAME uptr COMMAND uptr)
#add_test(NAME shdat COMMAND shdat)
add_test(NAME special COMMAND special)
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS special)
