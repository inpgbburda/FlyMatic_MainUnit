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
}
Acc_Axis_T;

typedef enum
{
    ROLL,
    PITCH,
    MAX_ANGLE_AXIS_NUM
}
Angle_Axis_T;

class SensorData {
public:
    std::map<Acc_Axis_T, int32_t> rawAccelerations_;
    std::map<Acc_Axis_T, int32_t> physicalAccelerations_;
    std::map<Angle_Axis_T, int32_t> spiritAngles_;
};


class Mpu6050Sensor
{
private: 
    I2c* i2c_handle_;
    SensorData& data_to_fill_;
    bool CheckPresence(void) const;
    int16_t ReadAcceleration(Acc_Axis_T axis) const;

public:
    Mpu6050Sensor(I2c* i2c_handle, SensorData& Data_): i2c_handle_(i2c_handle), data_to_fill_(Data_){};
    void ReadSensorData(void);
    void Start(void);
};


class Mpu6050AccConverter
{
private:
    SensorData& data_;

public:
    Mpu6050AccConverter(SensorData& data): data_(data){};
    void ConvertRawToPhysical(void);
};


class Mpu6050AngleConverter
{
private:
    SensorData& data_;
public:
    Mpu6050AngleConverter(SensorData& data): data_(data){};
    void CalculateSpiritAngles(void);
};


class Mpu6050 
{
private:
    I2c* i2c_handle_;
    SensorData sensorData_;

    Mpu6050Sensor sensor_;
    Mpu6050AccConverter accConverter_;
    Mpu6050AngleConverter angleConverter_;

public:
    Mpu6050(I2c* i2c_handle);
    void Init(void);
    void Start(void);
    bool HasValidI2cInstance(void) const;
    void SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc);
    int32_t GetSpiritAngle(Angle_Axis_T axis) const;
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    void ReadAndProcessSensorData(void);

    /*UT interfaces:*/
#ifdef _UNIT_TEST
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    int16_t GetRawAcceleration (Acc_Axis_T axis) const;
    void ReadSensorData(void);
    void ConvertRawToPhysical(void);
    void CalculateSpiritAngles(void);
#endif /*_UNIT_TEST*/
};
