#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "i2c.cpp"


TEST_CASE( "Test generating file descriptor", "[i2c]" ) 
{
    using Catch::Matchers::Equals;
    char filename[I2C_DRV_DESRC_MAX_FILE_L];

    ComposeDriverFilename(filename, 1);
    std::string filename_str(filename);

    REQUIRE_THAT(filename_str, Equals("/dev/i2c-1"));

}

TEST_CASE( "Test failing generating file descriptor - nullptr instead of string", "[i2c]" ) 
{
    char *filename = nullptr;
    REQUIRE_THROWS( ComposeDriverFilename(filename, 1) );
}

TEST_CASE( "Driver number out of range", "[i2c]" ) 
{
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    REQUIRE_THROWS( ComposeDriverFilename(filename, 10) );
    REQUIRE_THROWS( ComposeDriverFilename(filename, -3) );
}

TEST_CASE( "Driver initialization", "[i2c]" ) 
{
    DriverInit(1);

}