#include "mpu6050.hpp"
#include "i2c.hpp"
#include <vector>

#define ACC_H 0U
#define ACC_L 1U
#define ACC_SIZE 2U

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
}

bool Mpu6050::CheckPhysicalPresence(void) const
{
    int who_i_am = i2c_handle_->ReadByte(MPU6050_WHO_AM_I);
    return MPU6050_WHO_AM_I_VAL == who_i_am;
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
    else
    {
    }

    Acc_Bytes = i2c_handle_->ReadBlockOfBytes(start_reg, ACC_SIZE);
    acc = ((int16_t)(Acc_Bytes[ACC_H]) << 8) + (int16_t)Acc_Bytes[ACC_L];

    return acc;
}

bool Mpu6050::HasValidI2cInstance(void) const
{
    return nullptr != i2c_handle_;
}

Mpu6050::~Mpu6050()
{
}
