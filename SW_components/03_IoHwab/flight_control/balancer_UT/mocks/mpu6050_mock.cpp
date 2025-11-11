#include "mpu6050.hpp"

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

Mpu6050 mpu6050;

void Mpu6050::ProcessSensorData(void)
{
}

int32_t Mpu6050::GetSpiritAngle(Angle_Axis_T axis)
{
    mock().actualCall("GetSpiritAngle")
          .withParameter("axis", axis);
    return mock().intReturnValue();
}

void Mpu6050::ReadSensorData(void)
{
}