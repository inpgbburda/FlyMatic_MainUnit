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

TEST(Mpu6050, Starts)
{
    mpu6050->Start();
}

TEST(Mpu6050, AsksPhysicalSensorToReportItsPresence)
{
    mock().expectOneCall("ReadByte").andReturnValue(MPU6050_WHO_AM_I_VAL);
    CHECK(mpu6050->CheckPhysicalPresence());
}

TEST(Mpu6050, AsksPhysicalSensorThatIsNotPresent)
{
    const uint8_t arbitrary_invalid_value = 0;
    mock().expectOneCall("ReadByte").andReturnValue(arbitrary_invalid_value);
    CHECK_FALSE(mpu6050->CheckPhysicalPresence());
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