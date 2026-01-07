#Configuration of build is precised in CMakePresets.json file

cmake_minimum_required(VERSION 3.18.1)
project(balancer_UT)

SET(CPPUTEST_RLV_PATH 
    ../../../../Tools/Testing/cpp_utest
)

# Search for CppUTest include headers - system path (Linux-RPi) or local (Cygwin)
# Prefer system CppUTest headers when available (keeps version in sync with libs)
find_path(CPPUTEST_INCLUDE_DIR
    NAMES CppUTest/CommandLineTestRunner.h CppUTest/TestHarness.h
)
if(CPPUTEST_INCLUDE_DIR)
    set(CPPUTEST_INCLUDE_USE ${CPPUTEST_INCLUDE_DIR})
else()
    set(CPPUTEST_INCLUDE_USE ${CPPUTEST_RLV_PATH}/includes/)
endif()

add_executable(${CMAKE_PROJECT_NAME}
    balancer_UT.cpp
    run_all_tests.cpp
    mocks/mpu6050_mock.cpp
    mocks/Thread_Manager_mock.cpp
    mocks/spi_mock.cpp
    ../balancer.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CPPUTEST_INCLUDE_USE}
    ../
    stubs/
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE _UNIT_TEST)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wno-float-conversion
    -O0 -g --coverage
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE --coverage)

set(CPPUTEST_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs")

# Search for CppUTest libs headers - system path (Linux-RPi) or local (Cygwin)
# Try to find system libs first
find_library(CPPUTEST_LIB       NAMES CppUTest     libCppUTest)
find_library(CPPUTEST_EXT_LIB   NAMES CppUTestExt  libCppUTestExt)

# If not found, try to find local libs
if(NOT CPPUTEST_LIB)
    find_library(CPPUTEST_LIB       NAMES CppUTest     libCppUTest     PATHS "${CPPUTEST_LIBDIR}" NO_DEFAULT_PATH)
endif()
if(NOT CPPUTEST_EXT_LIB)
    find_library(CPPUTEST_EXT_LIB   NAMES CppUTestExt  libCppUTestExt  PATHS "${CPPUTEST_LIBDIR}" NO_DEFAULT_PATH)
endif()
if(NOT CPPUTEST_LIB OR NOT CPPUTEST_EXT_LIB)
    message(FATAL_ERROR "CppUTest libraries not found. Install or rebuild them for this platform/toolchain.")
endif()

# Final - link everything together
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        ${CPPUTEST_EXT_LIB}
        ${CPPUTEST_LIB}
)
