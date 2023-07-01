#pragma once

#include <cstdint>

#include "i2c.hpp"
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
    void Init(I2c* i2c_ptr);
    void Start(void);
    bool CheckPhysicalPresence(void) const;
    bool HasValidI2cInstance(void) const;
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    ~Mpu6050();
};