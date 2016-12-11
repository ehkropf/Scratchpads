cmake_minimum_required(VERSION 3.7)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
add_executable(bingap bingap.cpp)
add_executable(cycrot cycrot.cpp)
add_executable(oddocc oddoc.cpp)
add_executable(eqsum eqsum.cpp)

###########################################################################
add_test(NAME run_bingap COMMAND bingap)
add_custom_target(check
    COMMAND "${CMAKE_CTEST_COMMAND}"
    DEPENDS bingap
    )
