#Configuration of build is precised in CMakePresets.json file

cmake_minimum_required(VERSION 3.18.1)
project(mpu6050_UT)
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

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(${CMAKE_PROJECT_NAME}
    mpu6050_UT.cpp
    run_all_tests.cpp
    ../mpu6050.cpp
    mocks/i2c_mock.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTestExt.a
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTest.a
)
