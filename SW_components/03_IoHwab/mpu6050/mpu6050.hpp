#pragma once

#include <cstdint>

#include "i2c.hpp"
typedef enum
{
    X,
    Y,
    Z
}Acc_Axis_T;

class Mpu6050
{
private:
    /* data */
    I2c* i2c_handle_;
    int32_t raw_acc_value_;

public:
    Mpu6050(/* args */);
    void Init(I2c* i2c_ptr);
    void Start(void);
    bool CheckPhysicalPresence(void) const;
    bool HasValidI2cInstance(void) const;
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    int32_t GetPhysicalAcceleration(void) const;
    ~Mpu6050();
};