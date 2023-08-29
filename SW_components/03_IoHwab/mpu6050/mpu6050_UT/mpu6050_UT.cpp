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

const uint8_t Mpu6050_Who_Am_I = 0x75;
const uint8_t Mpu6050_I2c_Addr = 0x68;
const uint8_t Acc_Size = 2U;

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
        .withParameter("slave_addr", Mpu6050_I2c_Addr)
        .withParameter("addr", Mpu6050_Who_Am_I)
        .andReturnValue(Mpu6050_Who_Am_I_Val);

    mock().expectOneCall("WriteByte")
        .withParameter("slave_addr", Mpu6050_I2c_Addr)
        .withParameter("addr", Mpu6050_Pwr_Mgmt_1)
        .withParameter("data", Mpu6050_Pwr_Mgmt_1_Wake_Up);

    mpu6050->Start();
}

TEST(Mpu6050, CannotDetectPhysicalSensor)
{
    const uint8_t Arbitrary_Incorrect_Value = 0xAB;

    mock().expectOneCall("ReadByte")
        .withParameter("slave_addr", Mpu6050_I2c_Addr)
        .withParameter("addr", Mpu6050_Who_Am_I)
        .andReturnValue(Arbitrary_Incorrect_Value);

    mpu6050->Start();
}

TEST(Mpu6050, ReadsAccelerationInXAxis)
{
    const uint8_t Mpu6050_Accel_Xout_H = 0x3BU;
    std::vector<uint8_t> Minus_Ten_In_U2 = {0xFF,0xF6};

    mock().expectOneCall("ReadBlockOfBytes")
        .withParameter("slave_addr", Mpu6050_I2c_Addr)
        .withParameter("start_reg_addr", Mpu6050_Accel_Xout_H)
        .withParameter("block_len", Acc_Size)
        .andReturnValue(&Minus_Ten_In_U2);

    int16_t x_acc = mpu6050->ReadAccceleration(X);

    CHECK_EQUAL(x_acc, -10);
}

TEST(Mpu6050, ReadsAccelerationInYAxis)
{
    const uint8_t Mpu6050_Accel_Yout_H = 0x3DU;
    std::vector<uint8_t> Twenty_In_U2 = {0x00,0x14};

    mock().expectOneCall("ReadBlockOfBytes")
        .withParameter("slave_addr", Mpu6050_I2c_Addr)
        .withParameter("start_reg_addr", Mpu6050_Accel_Yout_H)
        .withParameter("block_len", Acc_Size)
        .andReturnValue(&Twenty_In_U2);

    int16_t y_acc = mpu6050->ReadAccceleration(Y);

    CHECK_EQUAL(y_acc, 20);
}

TEST(Mpu6050, ReadsAccelerationInZAxis)
{
    const uint8_t Mpu6050_Accel_Zout_H = 0x3FU;
    std::vector<uint8_t> Minus_Thirty_In_U2 = {0xFF,0xE2};

    mock().expectOneCall("ReadBlockOfBytes")
            .withParameter("slave_addr", Mpu6050_I2c_Addr)
            .withParameter("start_reg_addr", Mpu6050_Accel_Zout_H)
            .withParameter("block_len", Acc_Size)
            .ignoreOtherParameters()
            .andReturnValue(&Minus_Thirty_In_U2);
        
    int16_t z_acc = mpu6050->ReadAccceleration(Z);

    CHECK_EQUAL(z_acc, -30);
}

/*
* Below tests assume:
*    physicalAcc = rawData * 2g / int16_t_MAX * 1000  
*/

TEST(Mpu6050, ConvertsPositiveRawReadingsToPhysicalAccelerationInXAxis)
{
    const int rawAccelerationX = 2000;
    const int expectedPhysicalAccelerationX = 122;

    mpu6050->SetRawAcceleration(X, rawAccelerationX);
    mpu6050->ConvertReadings();
    CHECK_EQUAL(mpu6050->GetPhysicalAcceleration(X), expectedPhysicalAccelerationX);
}

TEST(Mpu6050, ConvertsNegativeRawReadingsToPhysicalAccelerationInXAxis)
{
    const int rawAccelerationX = -600;
    const int expectedPhysicalAccelerationX = -36;

    mpu6050->SetRawAcceleration(X, rawAccelerationX);
    mpu6050->ConvertReadings();
    CHECK_EQUAL(mpu6050->GetPhysicalAcceleration(X), expectedPhysicalAccelerationX);
}

// TEST(Mpu6050, ConvertsMinimalPositiveRawReadingsToPhysicalAccelerationInXAxis)
// {
//     mpu6050->SetRawAcceleration(X, 1);
//     CHECK_EQUAL(mpu6050->GetPhysicalAcceleration(X), 0);
// }

// TEST(Mpu6050, ConvertsRawReadingsToPhysicalAccelerationInYAxisAndIgnoresChengesInOtherAxis)
// {
//     const int16_t Arbitrary_Raw_Value = 2137;
    
//     mpu6050->SetRawAcceleration(Y, -32767);
//     mpu6050->SetRawAcceleration(X, Arbitrary_Raw_Value);

//     CHECK_EQUAL(mpu6050->GetPhysicalAcceleration(Y), -2000);
// }

TEST(Mpu6050, ConvertsMaxRawReadingsToPhysicalAccelerationInZAxis)
{
    const int rawAccelerationX = 32767;
    const int expectedPhysicalAccelerationX = 2000;

    mpu6050->SetRawAcceleration(Z, rawAccelerationX);
    mpu6050->ConvertReadings();
    CHECK_EQUAL(mpu6050->GetPhysicalAcceleration(Z), expectedPhysicalAccelerationX);
}