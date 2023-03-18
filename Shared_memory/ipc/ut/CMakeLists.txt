project(Shared_Memory_UT)
cmake_minimum_required(VERSION 3.16)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
find_package(Catch2 3 REQUIRED)
add_executable(Shared_Memory shm_UT.cpp ../shared_memory/flag.cpp ../shared_memory/shared_memory.cpp ../shared_memory/shared_memory_utils.cpp)
add_executable(Flag_class flag_UT.cpp ../shared_memory/flag.cpp)
target_link_libraries(Shared_Memory PRIVATE -lrt Catch2::Catch2WithMain) #order of lpq and libs matters
target_link_libraries(Flag_class PRIVATE -lrt Catch2::Catch2WithMain) 