# About
This is the **MainUnit** of the **FlyMatic** drone project.  
Click here to read more: [Flymatic_HomePage](https://inpgbburda.github.io/FlyMatic/)

## Commands
Before first compilation, run `Tools/Env/setup_rpi.sh` script. It will install and configure all needed libs and settings.

To run cmake generation:
    $ cmake -B ../Builds/

To generate Cmake for unit tests:

    1) Open cli at ..UT directory
    2) $ cmake -S . --preset=default