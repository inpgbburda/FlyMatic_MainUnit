#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "i2c.hpp"

#define I2C_DRV_DESRC_MAX_FILE_L 10

class MyClass
{
private:
    void MyMethod(){};

    friend class TEST_GROUP_CppUTestGroupMyClass;
};

TEST_GROUP(MyClass)
{
    void CallMyMethod( MyClass &o )
    {
        o.MyMethod();
    }
    void teardown()
    {
        mock().clear();
    }
};

void productionCode()
{
    mock().actualCall("productionCode");
}

TEST(MyClass, SimpleScenario)
{
    mock().expectOneCall("productionCode");
    productionCode();
    productionCode();
    mock().checkExpectations();
}

TEST(MyClass, FirstTest)
{
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    ComposeDriverFilename(filename, 1);
    std::string filename_str(filename);
    CHECK_EQUAL(filename_str, "/dev/i2c-1");
    
    MyClass testObj;
    CallMyMethod( testObj );
}

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