project(Testcase01)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
add_executable(Testcase01  ../main.cpp ../../shared_memory/flag.cpp ../../shared_memory/shared_memory.cpp ../../shared_memory/shared_memory_utils.cpp)
target_link_libraries(Testcase01 PRIVATE -lrt -lpthread) #order of lpq and libs matters