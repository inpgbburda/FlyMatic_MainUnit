# toolchain-rpi.cmake — Cross-compile for Raspberry Pi on Windows

# 1. Specify system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# 2. Toolchain prefix (adjust if needed)
set(TOOLCHAIN_PATH "D:/Programy/ARM_toolchain/SysGCC/bin")
SET(TOOLCHAIN_PREFIX arm-linux-gnueabihf-)

# Compilers
set(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++.exe)
set(CMAKE_AR           ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}ar.exe)
set(CMAKE_STRIP        ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}strip.exe)

# Optional: make CMake find headers/libs only inside sysroot (if any)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
