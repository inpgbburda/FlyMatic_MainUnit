#Configuration of build is precised in CMakePresets.json file

cmake_minimum_required(VERSION 3.18.1)
project(Thread_Manager_UT)
find_package(Catch2 3 REQUIRED)
message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# These tests can use the Catch2-provided main

include_directories(../)

add_definitions ( -D_UNIT_TEST )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(Thread_Manager_UT
    Thread_Manager_UT.cpp
    ../Thread_Manager.cpp
)

target_link_libraries(Thread_Manager_UT PRIVATE Catch2::Catch2WithMain)
