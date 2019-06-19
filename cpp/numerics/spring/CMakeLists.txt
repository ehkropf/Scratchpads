cmake_minimum_required(VERSION 3.6)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(Boost 1.66 REQUIRED COMPONENTS program_options)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17")

###########################################################################
add_executable(spring_calc spring_calc.cpp)
target_include_directories(spring_calc PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(spring_calc Boost::program_options)

###########################################################################
add_test(NAME spring_fwd COMMAND spring_fwd)
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS sprint_fwd)
