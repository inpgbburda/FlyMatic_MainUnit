#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "i2c.hpp"

#define I2C_DRV_DESRC_MAX_FILE_L 10

TEST_GROUP(I2c)
{
    void teardown()
    {
        mock().clear();
    }
};

TEST(I2c, ComposeDriverFilename)
{
    I2c i2cObj;
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    i2cObj.ComposeDriverFilename(filename, 1);
    std::string filename_str(filename);
    CHECK_EQUAL(filename_str, "/dev/i2c-1");
}


TEST(I2c, SetSlaveAddr)
{
    mock().expectOneCall("ioctl");
    I2c i2cObj;
    i2cObj.SetSlaveAddr(0xABCD);
    mock().checkExpectations();
}


TEST(I2c, ReadByte_Ok)
{
    int reg_val;
    I2c i2cObj;
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(10);
    reg_val = i2cObj.ReadByte(reg_addr);
    CHECK_EQUAL(reg_val, 10);
    mock().checkExpectations();
}

TEST(I2c, ReadByte_Nok)
{
    int reg_val;
    I2c i2cObj;
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(-10);
    /* Reading byte faulty */
    CHECK_THROWS(std::exception, i2cObj.ReadByte(reg_addr));
    mock().checkExpectations();
}
