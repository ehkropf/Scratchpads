cmake_minimum_required(VERSION 3.7)

# enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
add_executable(ex1 ex1.cpp)

set(RNAME ex1)

###########################################################################
# add_test(NAME run COMMAND bingap)
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${RNAME}
    DEPENDS ${RNAME}
    )
