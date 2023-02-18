cmake_minimum_required(VERSION 3.18.1)
project(drone_code)

message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

set(SOURCES 
    flight_control/pwm.cpp
    services/Thread_Manager.cpp
)

set(HEADERS
    flight_control
    services
)

option(_RASP "Compile for Raspbberry" OFF)
if (_RASP)
  add_definitions(-D_RASP)
endif()

include_directories(${HEADERS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable (executable ${SOURCES} main.cpp)

if (_RASP)

  find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
  target_link_libraries(executable ${WIRINGPI_LIBRARIES})

endif()
