#pragma once

#include <cstdint>

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
public:
    Mpu6050(/* args */);
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    ~Mpu6050();
};