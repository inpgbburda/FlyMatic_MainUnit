#pragma once

#include <cstdint>
typedef enum
{
    X = 0,
    Y,
    Z,
    MAX_AXIS_NUMBER
}Acc_Axis_T;

class Mpu6050
{
public:
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    void MainFunc(void);
};