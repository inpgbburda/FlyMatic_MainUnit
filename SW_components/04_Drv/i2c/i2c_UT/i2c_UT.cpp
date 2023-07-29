#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "i2c.hpp"
#include <vector>

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

const uint8_t arbitrary_reg_addr = 0xAA;
const uint8_t arbitrary_slv_addr = 0xBB;

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

TEST(I2c, SetsSlaveAddr)
{
    mock().expectOneCall("ioctl").andReturnValue(0);

    CHECK_EQUAL(i2cObj->GetCurrentSlaveAddr(), INVALID_SLAVE_ADDR);
    i2cObj->SetSlaveAddr(0xAA);
    CHECK_EQUAL(i2cObj->GetCurrentSlaveAddr(), 0xAA);
}

TEST(I2c, ReadsByte)
{
    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(10);
    mock().ignoreOtherCalls();
    i2cObj->Init(DRV_6);

    uint8_t obtained_byte = i2cObj->ReadByte(arbitrary_slv_addr, arbitrary_reg_addr);

    CHECK_EQUAL(obtained_byte, 10);
}

TEST(I2c, FailsReadingByte)
{
    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(-1);
    mock().ignoreOtherCalls();

    i2cObj->Init(DRV_6);
    CHECK_THROWS(std::exception, i2cObj->ReadByte(arbitrary_slv_addr, arbitrary_reg_addr));
}

TEST(I2c, ReadsBlockOfFourBytes)
{
    uint8_t Exp_Bytes[] = {0x01, 0x05, 0x16, 0x08};
    
    mock().expectOneCall("i2c_smbus_read_i2c_block_data")
        .withOutputParameterReturning("values", Exp_Bytes, sizeof(Exp_Bytes))
        .ignoreOtherParameters();

    MEMCMP_EQUAL(i2cObj->ReadBlockOfBytes(arbitrary_reg_addr, 4).data(), Exp_Bytes, 4);
}

TEST(I2c, ReadsBlockOfTwoBytes)
{
    uint8_t Exp_Bytes[] = {0x01, 0x08};

    mock().expectOneCall("i2c_smbus_read_i2c_block_data")
        .withOutputParameterReturning("values", Exp_Bytes, sizeof(Exp_Bytes))
        .ignoreOtherParameters();
        
    std::vector<uint8_t> Obtained_Bytes = i2cObj->ReadBlockOfBytes(arbitrary_reg_addr, 2);

    MEMCMP_EQUAL(Obtained_Bytes.data(), Exp_Bytes, 2);
}