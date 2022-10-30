find_package(Catch2 3 REQUIRED)

# These tests can use the Catch2-provided main

include_directories(../SW_components/flight_control)
add_executable(tests 
    test.cpp
    ../SW_components/flight_control/pwm.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
