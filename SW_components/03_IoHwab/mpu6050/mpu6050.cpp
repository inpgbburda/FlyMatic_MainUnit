#include "mpu6050.hpp"
#include "i2c.hpp"

#include <vector>
#include <stdexcept>

#define MPU6050_I2R_ADDR 0x68U

#define WHO_AM_I 0x75U
#define WHO_AM_I_VAL 0x68U

#define PWR_MGMT_1 0x6BU
#define PWR_MGMT_1_WAKE_UP 0x00U

#define ACCEL_XOUT_H 0x3BU
#define ACCEL_YOUT_H 0x3DU
#define ACCEL_ZOUT_H 0x3FU

#define ACC_H 0U
#define ACC_L 1U
#define ACC_SIZE 2U

#define INT16_T_MAX_VAL 32767
#define ACC_MAX_VAL 2

I2c i2c_bus;

Mpu6050::Mpu6050():i2c_handle_(nullptr)
{
}

void Mpu6050::Init(I2c* i2c_ptr)
{
    i2c_handle_ = i2c_ptr;
}

void Mpu6050::Start(void)
{
    bool sensor_detected = CheckPhysicalPresence();
    if(sensor_detected)
    {
        i2c_handle_ -> WriteByte(MPU6050_I2R_ADDR, PWR_MGMT_1, PWR_MGMT_1_WAKE_UP);
    }
}

bool Mpu6050::CheckPhysicalPresence(void) const
{
    int who_i_am = i2c_handle_->ReadByte(MPU6050_I2R_ADDR, WHO_AM_I);
    return WHO_AM_I_VAL == who_i_am;
}

int16_t Mpu6050::ReadAccceleration(Acc_Axis_T axis) const
{
    std::vector<uint8_t> Acc_Bytes;
    int16_t acc = 0;
    uint8_t start_reg = 0x00;

    if(X == axis)
    {
        start_reg = ACCEL_XOUT_H;
    }
    else if(Y == axis)
    {
        start_reg = ACCEL_YOUT_H;
    }
    else if(Z == axis)
    {
        start_reg = ACCEL_ZOUT_H;
    }
    else
    {
    }

    Acc_Bytes = i2c_handle_->ReadBlockOfBytes(MPU6050_I2R_ADDR, start_reg, ACC_SIZE);
    acc = ((int16_t)(Acc_Bytes[ACC_H]) << 8) + (int16_t)Acc_Bytes[ACC_L];

    return acc;
}

void Mpu6050::SetRawAcceleration(Acc_Axis_T axis, int16_t acc)
{
    Raw_Accelerations_[axis] = acc;
}

int32_t Mpu6050::GetPhysicalAcceleration(Acc_Axis_T axis) const
{
    return Physical_Accelerations_[axis];
}

bool Mpu6050::HasValidI2cInstance(void) const
{
    return nullptr != i2c_handle_;
}

void Mpu6050::ConvertReadings(void)
{
    for(int i=0; i<MAX_AXIS_NUMBER; i++)
    {
        int32_t raw_value = Raw_Accelerations_[i];
        Physical_Accelerations_[i] = raw_value * ACC_MAX_VAL * 1000 / INT16_T_MAX_VAL;
    }
}

Mpu6050::~Mpu6050()
{
}
