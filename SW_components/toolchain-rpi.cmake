# toolchain-rpi.cmake — Cross-compile for Raspberry Pi on Windows

# 1. Specify system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# 2. Toolchain prefix (adjust if needed)
SET(TOOLCHAIN_PREFIX arm-linux-gnueabihf-)

# 3. Compiler tools
SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
SET(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)
SET(CMAKE_LINKER       ${TOOLCHAIN_PREFIX}ld)
SET(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy)
SET(CMAKE_OBJDUMP      ${TOOLCHAIN_PREFIX}objdump)
SET(CMAKE_STRIP        ${TOOLCHAIN_PREFIX}strip)

# 4. Optional: paths to headers/libraries on target (e.g., sysroot)
# set(CMAKE_FIND_ROOT_PATH /path/to/raspi/sysroot)

# 5. Search settings
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
