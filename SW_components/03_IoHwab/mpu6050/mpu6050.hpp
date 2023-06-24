#pragma once

#include <cstdint>

#include "i2c.hpp"

#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D

typedef enum
{
    X,
    Y
}Acc_Axis_T;

class Mpu6050
{
private:
    /* data */
    I2c* i2c_handle_;

public:
    Mpu6050(/* args */);
    bool hasValidI2cInstance(void) const;
    void Init(I2c* i2c_ptr);
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    ~Mpu6050();
};