#include "mpu6050.hpp"
#include "i2c.hpp"
#include <vector>

I2c i2c_bus;

Mpu6050::Mpu6050():i2c_handle_(nullptr)
{
}

void Mpu6050::Init(I2c* i2c_ptr)
{
    i2c_handle_ = i2c_ptr;
}

int16_t Mpu6050::ReadAccceleration(Acc_Axis_T axis) const
{
    std::vector<uint8_t> Acc_Bytes;
    int16_t acc = 0;

    if(X == axis)
    {
        Acc_Bytes = i2c_handle_->ReadBlockOfBytes(ACCEL_XOUT_H, 2);
        acc = ((int16_t)(Acc_Bytes[0]) << 8) + (int16_t)Acc_Bytes[1];
    }
    else if(Y == axis)
    {
        Acc_Bytes = i2c_handle_->ReadBlockOfBytes(ACCEL_YOUT_H, 2);
        acc = ((int16_t)(Acc_Bytes[0]) << 8) + (int16_t)Acc_Bytes[1];
    }
    else
    {
    }
    return acc;
}

bool Mpu6050::hasValidI2cInstance(void) const
{
    return nullptr != i2c_handle_;
}

Mpu6050::~Mpu6050()
{
}
