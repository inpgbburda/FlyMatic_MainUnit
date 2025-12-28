# About
This is the **MainUnit** of the **FlyMatic** drone project.  
Click here to read more: [Flymatic_HomePage](https://inpgbburda.github.io/FlyMatic/)

Needed to install on PC machine
- **cygwin** -> for UT buiding, with the following packs
    - cmake
    - make
    - gcc/g++
    - gcov
    - python3 
    - pip3 -> gcovr 

Install it under `C:\cygwin64` and add it to system path.  
The **VScode Tasks** are automatically configured to run under cygwin and the `Cygwin` terminal profile is created (adjust paths in `settings.json`).

## Commands
Before first compilation on RPI, run `Tools/Env/setup_rpi.sh` script. It will install and configure all needed libs and settings.

To run cmake generation:
    `$ cmake -B ../Builds/`

To generate Cmake for unit tests:

1) Open cli at ..UT directory
2) $ `cmake -S . --preset=default`