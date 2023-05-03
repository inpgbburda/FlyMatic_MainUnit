#Configuration of build is precised in CMakePresets.json file

cmake_minimum_required(VERSION 3.18.1)
project(i2c_UT)
message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

SET(CPPUTEST_RLV_PATH 
    ../../../Tools/Testing/cpp_utest
)

include_directories(
    ${CPPUTEST_RLV_PATH}/includes/
    ../
    stubs/
)

add_definitions ( -D_UNIT_TEST )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(i2c_UT
    i2c_UT.cpp
    run_all_tests.cpp
    ../i2c.cpp
    mocks/smbus_mock.cpp
    mocks/ioctl_mock.cpp
)

target_link_libraries(i2c_UT 
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTestExt.a
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPUTEST_RLV_PATH}/libs/libCppUTest.a       
)
