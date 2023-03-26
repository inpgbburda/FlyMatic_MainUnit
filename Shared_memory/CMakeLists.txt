project(Shared_Memory)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

add_executable(Shared_Memory main.cpp)

target_link_libraries(Shared_Memory PRIVATE -lrt) #order of lpq and libs matters