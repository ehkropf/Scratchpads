cmake_minimum_required(VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "-Wall -std=c++14")

###########################################################################
# Things to make.
#

add_executable(bttop bttop.cpp)

add_executable(btbfs btbfs.cpp)

add_executable(btswap btswap.cpp)

add_executable(swapsub swapsub.cpp)

add_executable(tcalc tcalc.cpp)

add_custom_target(run cat ${CMAKE_SOURCE_DIR}/ex/kitty/input/input00.txt | ${CMAKE_BINARY_DIR}/tcalc
    COMMAND echo '-----'
    COMMAND cat ${CMAKE_SOURCE_DIR}/ex/kitty/output/output00.txt
    COMMAND echo
    DEPENDS tcalc
    )
