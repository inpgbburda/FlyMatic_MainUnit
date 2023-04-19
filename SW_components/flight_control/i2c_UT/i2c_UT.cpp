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


TEST(I2c, FirstTest)
{
    I2c i2cObj;
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    i2cObj.ComposeDriverFilename(filename, 1);
    std::string filename_str(filename);
    CHECK_EQUAL(filename_str, "/dev/i2c-1");
}


// TEST(I2c_Init fault, FirstTest)
// {
//     // expect ComposeDriverFilename()
//     // expect OpenDriverFile
//     // *błąd initu drivera* //
//     //CHECK(exception is thrown);
// }


TEST(I2c, Read_Byte_Ok)
{
    int reg_val;
    I2c i2cObj;
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(10);
    reg_val = i2cObj.ReadByte(reg_addr);
    CHECK_EQUAL(reg_val, 10);
    mock().checkExpectations();
}

TEST(I2c, Read_Byte_Nok)
{
    int reg_val;
    I2c i2cObj;
    int reg_addr = 30;

    mock().expectOneCall("i2c_smbus_read_byte_data").andReturnValue(-10);
    /* Reading byte faulty */
    CHECK_THROWS(std::exception, i2cObj.ReadByte(reg_addr));
    mock().checkExpectations();
}

// TEST(I2c_Read_Bytes, FirstTest)
// {
//     //result = I2c_Read_Bytes(start addr, len)
//     //expect i2c_smbus_read_byte_data( rejestr)
//     // *błąd odczytu* //
//     //CHECK(exception is thrown);
// }