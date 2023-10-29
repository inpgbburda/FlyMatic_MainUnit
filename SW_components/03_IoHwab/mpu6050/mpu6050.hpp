#pragma once

#include <cstdint>
#include <map>

#include "i2c.hpp"
typedef enum
{
    X = 0,
    Y,
    Z,
    MAX_AXIS_NUMBER
}Acc_Axis_T;

typedef enum
{
    ROLL,
    PITCH,
    MAX_ANGLE_AXIS_NUM
}
Angle_Axis_T;

class Mpu6050
{
private:
    /* data */
    I2c* i2c_handle_;
     std::map<Acc_Axis_T, int32_t> Raw_Accelerations_
    {
        {X, 0},
        {Y, 0},
        {Z, 0}
    };
    std::map<Acc_Axis_T, int32_t> Physical_Accelerations_
    {
        {X, 0},
        {Y, 0},
        {Z, 0}
    };
    std::map<Angle_Axis_T, int32_t> Spirit_Angles_
    {
        {ROLL, 0},
        {PITCH, 0}
    };

public:
    Mpu6050(/* args */);
    void Init(I2c* i2c_ptr);
    void Start(void);
    void MainFunc(void);
    bool CheckSensorPresence(void) const;
    bool HasValidI2cInstance(void) const;
    int16_t ReadAccceleration(Acc_Axis_T axis) const;
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    void SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc);
    void CalculateSpiritAngles(void);
    int32_t GetSpiritAngle(Angle_Axis_T axis) const;
    void ConvertAccelerations(void);
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    ~Mpu6050();
};