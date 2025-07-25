cmake_minimum_required(VERSION 3.18.1)
project(drone_code LANGUAGES CXX)

# -------------------------------------------------------------------
# 1. Raspberry option / cross‑toolchain setup
# -------------------------------------------------------------------
option(_RASP "Compile for Raspberry Pi (use cross-toolchain)" OFF)
if(_RASP)
  # If no toolchain is specified externally, set the default path
  if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain-rpi.cmake"
      CACHE PATH "toolchain file for Raspberry Pi" FORCE)
  endif()
endif()

# -------------------------------------------------------------------
# 2. Sources and header directories
# -------------------------------------------------------------------
set(SOURCES
    02_Srv/main.cpp
    02_Srv/threads/Thread_Manager.cpp
    02_Srv/threads/Thread_Manager_Cfg.cpp
    03_IoHwab/mpu6050/mpu6050.cpp
    03_IoHwab/flight_control/balancer.cpp
    04_Drv/i2c/i2c.cpp
    04_Drv/spi/spi.cpp
)

# (These are directories containing header files, not the files themselves)
set(HEADER_DIRS
    02_Srv/threads
    03_IoHwab/mpu6050
    03_IoHwab/flight_control
    04_Drv/i2c
    04_Drv/spi
)
include_directories(${HEADER_DIRS})

# -------------------------------------------------------------------
# 3. Compiler flags
# -------------------------------------------------------------------
# Common flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pthread")
# Debug / Release specific flags
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# -------------------------------------------------------------------
# 4. Executable + link wiringPi / I2C
# -------------------------------------------------------------------
set(EXEC_NAME "DroneExec")
add_executable(${EXEC_NAME} ${SOURCES})


if(_RASP)
  set(WIRINGPI_LIBRARIES "${CMAKE_SYSROOT}/usr/lib/libwiringPi.so")
  target_link_libraries(${EXEC_NAME}
    PRIVATE
      ${WIRINGPI_LIBRARIES}
      i2c
  )

  # Print size after build
  add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
    COMMAND size --format=berkeley --totals "$<TARGET_FILE:${EXEC_NAME}>"
    COMMENT "Print binary size"
  )
endif()

# -------------------------------------------------------------------
# 5. Deploy / debug-server as custom targets
# -------------------------------------------------------------------
if(_RASP)
  # --- User-defined variables (change to match your setup) ---
  set(PI_USER "pi")
  set(PI_HOST "raspberrypi.local")
  set(PI_APP_DIR "/home/pi/my_app")

  # Deploy target – sync via rsync
  add_custom_target(deploy
    COMMAND ${CMAKE_COMMAND} -E echo "Deploying to ${PI_HOST}..."
    COMMAND rsync -avz
            "$<TARGET_FILE:${EXEC_NAME}>"
            ${PI_USER}@${PI_HOST}:${PI_APP_DIR}/${EXEC_NAME}
    COMMENT "Deploying ${EXEC_NAME} to Raspberry Pi"
    DEPENDS ${EXEC_NAME}
    VERBATIM
  )

  # Debug-server target – run gdbserver via SSH
  add_custom_target(debug-server
    COMMAND ${CMAKE_COMMAND} -E echo "Starting gdbserver on ${PI_HOST}:2345..."
    COMMAND ssh ${PI_USER}@${PI_HOST}
            "cd ${PI_APP_DIR} && gdbserver --once :2345 ./${EXEC_NAME}"
    DEPENDS deploy
    VERBATIM
  )
endif()
