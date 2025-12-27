#Configuration of build is precised in CMakePresets.json file

cmake_minimum_required(VERSION 3.18.1)
project(balancer_UT)
message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

SET(CPPUTEST_RLV_PATH 
    ../../../../Tools/Testing/cpp_utest
)

include_directories(
    ${CPPUTEST_RLV_PATH}/includes/
    ../
    stubs/
)

add_definitions ( -D_UNIT_TEST )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wno-float-conversion")
add_compile_options(-O0 -g --coverage)
add_link_options(--coverage)

add_executable(${CMAKE_PROJECT_NAME}
    balancer_UT.cpp
    run_all_tests.cpp
    mocks/mpu6050_mock.cpp
    mocks/Thread_Manager_mock.cpp
    mocks/spi_mock.cpp
    ../balancer.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTestExt.a
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTest.a
)
