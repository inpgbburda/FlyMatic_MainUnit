#pragma once

#include <cstdint>

#include "i2c.hpp"
typedef enum
{
    X = 0,
    Y,
    Z,
    MAX_AXIS_NUMBER
}Acc_Axis_T;

class Mpu6050
{
private:
    /* data */
    I2c* i2c_handle_;
    int32_t Raw_Accelerations_[MAX_AXIS_NUMBER];
    int32_t Physical_Accelerations_[MAX_AXIS_NUMBER] = {0};

public:
    Mpu6050(/* args */);
    void Init(I2c* i2c_ptr);
    void Start(void);
    bool CheckSensorPresence(void) const;
    bool HasValidI2cInstance(void) const;
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    void ConvertReadings(void);
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    ~Mpu6050();
};