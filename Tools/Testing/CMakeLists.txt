cmake_minimum_required(VERSION 3.18.1)
project(testCmake)
find_package(Catch2 3 REQUIRED)
message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# These tests can use the Catch2-provided main

include_directories(../SW_components/flight_control)

add_definitions ( -D_UNIT_TEST )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")

add_executable(tests 
    test.cpp
    EventAddress_UT.cpp
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
