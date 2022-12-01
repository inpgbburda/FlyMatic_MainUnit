cmake_minimum_required(VERSION 3.18.1)
project(drone_code)

set(SOURCES 
    flight_control/pwm.cpp
)

set(HEADERS
    flight_control
)

option(_RASP "Compile for Raspbberry" OFF)
if (_RASP)
  add_definitions(-D_RASP)
endif()

include_directories(${HEADERS})


add_compile_options(
       -Wall -Werror
)

add_executable (executable ${SOURCES} main.cpp)

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(executable ${WIRINGPI_LIBRARIES})