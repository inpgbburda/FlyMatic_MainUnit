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
    I2c testObj;
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    testObj.ComposeDriverFilename(filename, 1);
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
    I2c testObj;
    mock().expectOneCall("i2c_smbus_read_byte_data");
    testObj.ReadByte(10);
    mock().checkExpectations();
}

// TEST(faulty I2c_Read_Byte, FirstTest)
// {
//     //wynik = I2c_Read_Byte( rejestr);
//     //expect i2c_smbus_read_byte_data( rejestr)
//     // *błąd odczytu* //
//     //CHECK(exception is thrown);
// }

// TEST(I2c_Read_Bytes, FirstTest)
// {
//     //result = I2c_Read_Bytes(start addr, len)
//     //expect i2c_smbus_read_byte_data( rejestr)
//     // *błąd odczytu* //
//     //CHECK(exception is thrown);
// }