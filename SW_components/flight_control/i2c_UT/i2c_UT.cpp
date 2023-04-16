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

void productionCode()
{
    mock().actualCall("productionCode");
}

TEST(I2c, SimpleScenario_Mocking_function)
{
    mock().expectOneCall("productionCode");
    productionCode();
    productionCode();
    mock().checkExpectations();
}

TEST(I2c, FirstTest)
{
    I2c testObj;
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    testObj.ComposeDriverFilename(filename, 1);
    std::string filename_str(filename);
    CHECK_EQUAL(filename_str, "/dev/i2c-1");
}

// TEST(MyClass, MockPrivateMethod)
// {
//     mock().expectOneCall("MyMethod");
//     MyClass testObj;
//     CallMyMethod( testObj );
//     mock().checkExpectations();
// }
// TEST(I2c_Init, FirstTest)
// {
//     // expect ComposeDriverFilename()
//     // expect opendriverFile
// }

// TEST(I2c_Init fault, FirstTest)
// {
//     // expect ComposeDriverFilename()
//     // expect opendriverFile
//     // *błąd initu drivera* //
//     //CHECK(exception is thrown);
// }


// TEST(I2c_Read_Byte, FirstTest)
// {
//     //wynik = I2c_Read_Byte( rejestr);
//     //expect i2c_smbus_read_byte_data( rejestr)
//     //CHECK(wynik == oczekiwany wynik);
// }

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