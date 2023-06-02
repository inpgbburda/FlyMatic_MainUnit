#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "i2c.hpp"


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
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(I2c, InitsFirstDriver)
{
    mock().expectOneCall("open").withParameter("__path", "/dev/i2c-1").ignoreOtherParameters();

    i2cObj->Init(DRV_1);
}

TEST(I2c, InitsSixthDriver)
{
    mock().expectOneCall("open").withParameter("__path", "/dev/i2c-6").ignoreOtherParameters();

    i2cObj->Init(DRV_6);
}

TEST(I2c, ComposesDriverFilenameForSecondDriver)
{
    std::string filename_str = i2cObj->ComposeDriverFilename(DRV_2);
    CHECK_EQUAL(filename_str, std::string("/dev/i2c-2"));
}

TEST(I2c, SetsSlaveAddr)
{
    mock().expectOneCall("ioctl");

    i2cObj->SetSlaveAddr(0xABCD);
}

TEST(I2c, ReadsByte)
{
    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(10);

    CHECK_EQUAL(i2cObj->ReadByte(0xAA), 10);
}

TEST(I2c, FailsReadingByte)
{
    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(-10);

    CHECK_THROWS(std::exception, i2cObj->ReadByte(30));
}
#include <vector>

TEST(I2c, ReadsBlockOfFourBytes)
{
    std::vector<uint8_t> exp_bytes = {1, 5, 16, 8};

    MEMCMP_EQUAL(i2cObj->ReadBytes(100, 10).data(), exp_bytes.data(), 4);
}