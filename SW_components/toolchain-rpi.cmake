# toolchain-rpi.cmake — Cross-compile for Raspberry Pi on Windows

# 1. Specify target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PATH "" CACHE PATH "Path to cross-compilation toolchain (bin directory)")
set(RPI_SYSROOT "E:/Toolchains/RPi/sysroot")

# 2. Include user-local config if it exists
if(EXISTS "${CMAKE_SOURCE_DIR}/local-toolchain-config.cmake")
    include("${CMAKE_SOURCE_DIR}/local-toolchain-config.cmake")
endif()

# 3. Toolchain prefix
set(TOOLCHAIN_PREFIX arm-linux-gnueabihf-)

# Compilers
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++.exe)
set(CMAKE_AR           ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}ar.exe)
set(CMAKE_STRIP        ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}strip.exe)

# 4. Set sysroot if defined
if(RPI_SYSROOT)
    set(CMAKE_SYSROOT ${RPI_SYSROOT})
    set(CMAKE_STAGING_PREFIX ${RPI_SYSROOT})
endif()

# 5. Make CMake find headers/libs only in sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
