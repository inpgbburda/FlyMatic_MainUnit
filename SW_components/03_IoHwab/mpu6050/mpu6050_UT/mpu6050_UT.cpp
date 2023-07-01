#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "mpu6050.hpp"

#include <vector>

TEST_GROUP(Mpu6050)
{
    Mpu6050* mpu6050;

    void setup()
    {
        mpu6050 = new Mpu6050();
    }
    void teardown()
    {
        delete mpu6050;
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(Mpu6050, InitWithI2cInstance)
{
    I2c i2c;
    mpu6050->Init(&i2c);

    CHECK(mpu6050->HasValidI2cInstance());
}

TEST(Mpu6050, StartsProperly)
{
    const uint8_t Mpu6050_Who_Am_I = 0x75;
    const uint8_t Mpu6050_Who_Am_I_Val = 0x68;
    const uint8_t Mpu6050_Pwr_Mgmt_1 = 0x6B;
    const uint8_t Mpu6050_Pwr_Mgmt_1_Wake_Up = 0x00;

    mock().expectOneCall("ReadByte")
          .withParameter("addr", Mpu6050_Who_Am_I)
          .andReturnValue(Mpu6050_Who_Am_I_Val);

    mock().expectOneCall("WriteByte")
          .withParameter("addr", Mpu6050_Pwr_Mgmt_1)
          .withParameter("data", Mpu6050_Pwr_Mgmt_1_Wake_Up);

    mpu6050->Start();
}

TEST(Mpu6050, CannotDetectPhysicalSensor)
{
    const uint8_t Mpu6050_Who_Am_I = 0x75;
    const uint8_t Arbitrary_Value = 0x00;

    mock().expectOneCall("ReadByte")
          .withParameter("addr", Mpu6050_Who_Am_I)
          .andReturnValue(Arbitrary_Value);

    mpu6050->Start();
}


TEST(Mpu6050, ReadsAccelerationInXAxis)
{
    std::vector<uint8_t> Minus_Ten_In_U2 = {0xFF,0xF6};
    mock().expectOneCall("ReadBlockOfBytes").andReturnValue(&Minus_Ten_In_U2);

    int16_t x_acc = mpu6050->ReadAccceleration(X);

    CHECK_EQUAL(x_acc, -10);
}


TEST(Mpu6050, ReadsAccelerationInYAxis)
{
    std::vector<uint8_t> Ten_In_U2 = {0x00,0x14};
    mock().expectOneCall("ReadBlockOfBytes").andReturnValue(&Ten_In_U2);

    int16_t y_acc = mpu6050->ReadAccceleration(Y);

    CHECK_EQUAL(y_acc, 20);
}