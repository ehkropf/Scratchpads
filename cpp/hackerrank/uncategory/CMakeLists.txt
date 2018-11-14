cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

####################
# Set this.
#
set(EXNAME pickup)

###########################################################################
# Things to make.
#

add_executable(nck nCk.cpp)

add_executable(${EXNAME} ${EXNAME}.cpp)

add_custom_target(check
    COMMAND ${CMAKE_BINARY_DIR}/${EXNAME}
    DEPENDS ${EXNAME}
    )
