#pragma once

#include <cstdint>

typedef enum
{
    ROLL,
    PITCH,
    MAX_ANGLE_AXIS_NUM
}
Angle_Axis_T;

class Mpu6050
{
public:
    void ReadAndProcessSensorData(void);
    int32_t GetSpiritAngle(Angle_Axis_T axis);
};
