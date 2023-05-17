#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "i2c.hpp"

#define I2C_DRV_DESRC_MAX_FILE_L 10
#define O_RDWR 2U
/*
TODO:
- refactor Init

*/


TEST_GROUP(I2c)
{
    I2c* i2cObj;

    void setup()
    {
        i2cObj = new I2c();
    }
    void teardown()
    {
        delete i2cObj;
        mock().clear();
    }
};


TEST(I2c, InitsPropperlyFristDriver)
{
    mock().expectOneCall("open").withParameter("__path", "/dev/i2c-1").withParameter("__oflag", O_RDWR);
    i2cObj->Init(DRV_1);
    mock().checkExpectations();
}

TEST(I2c, InitsPropperlySixthDriver)
{
    mock().expectOneCall("open").withParameter("__path", "/dev/i2c-6").withParameter("__oflag", O_RDWR);
    i2cObj->Init(DRV_6);
    mock().checkExpectations();
}

TEST(I2c, ComposesDriverFilenameForSecondDriver)
{
    std::string filename_str = i2cObj->ComposeDriverFilename(DRV_2);
    CHECK_EQUAL(filename_str, std::string("/dev/i2c-2"));
}


TEST(I2c, SetSlaveAddr)
{
    mock().expectOneCall("ioctl");
    i2cObj->SetSlaveAddr(0xABCD);
    mock().checkExpectations();
}


TEST(I2c, ReadByte_Ok)
{
    int reg_val;
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(10);
    reg_val = i2cObj->ReadByte(reg_addr);
    CHECK_EQUAL(reg_val, 10);
    mock().checkExpectations();
}

TEST(I2c, ReadByte_Nok)
{
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(-10);
    /* Reading byte faulty */
    CHECK_THROWS(std::exception, i2cObj->ReadByte(reg_addr));
    mock().checkExpectations();
}
