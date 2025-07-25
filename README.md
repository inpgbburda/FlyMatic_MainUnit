# Set up

Configure paths in the `settings.json` file.  
Provide the compiler path in the in the `local-toolchain-config.cmake`.  
Install the `rsynch` on the git shell.

# Running
To run cmake generation:
    $ cmake -B ../Builds/

To generate Cmake for unit tests:

    1) Open cli at ..UT directory
    2) $ cmake -S . --preset=default

## Remote conntection
To connect to the uP run the command `ssh pi@raspberrypi.local`.