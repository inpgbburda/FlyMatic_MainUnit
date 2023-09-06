cmake_minimum_required(VERSION 3.18.1)
project(drone_code)

message("Used Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

set(SOURCES
    02_Srv/main.cpp
    02_Srv/threads/Thread_Manager.cpp
    02_Srv/threads/Thread_Manager_Cfg.cpp
    03_IoHwab/mpu6050/mpu6050.cpp
    03_IoHwab/flight_control/balancer.cpp
    04_Drv/pwm/pwm.cpp
    04_Drv/i2c/i2c.cpp
)

set(HEADERS
    02_Srv/threads/
    03_IoHwab/mpu6050/
    03_IoHwab/flight_control/
    04_Drv/pwm/
    04_Drv/i2c/
)

option(_RASP "Compile for Raspbberry" OFF)
if (_RASP)
  add_definitions(-D_RASP)
endif()

include_directories(${HEADERS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

SET(EXEC_NAME "DroneExec")

add_executable (${EXEC_NAME} ${SOURCES})

if (_RASP)

  find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
  target_link_libraries(${EXEC_NAME}
                        ${WIRINGPI_LIBRARIES}
                        i2c
   )

# Print size of linked binary file
add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
    COMMAND size --format=berkeley --totals "${EXEC_NAME}"
    COMMENT "Invoking: GNU Print Size")

endif()
