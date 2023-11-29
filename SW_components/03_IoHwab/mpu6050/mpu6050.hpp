/**
* File contains abstraction layer for MPU6050 sensor
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/
#pragma once

#include <cstdint>
#include <map>

#include "i2c.hpp"

/*
|===================================================================================================================================|
    Exported types declarations
|===================================================================================================================================|
*/
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

/*
|===================================================================================================================================|
    Exported objects declarations
|===================================================================================================================================|
*/
class SensorData {
public:
    std::map<Acc_Axis_T, int32_t> raw_accelerations_;
    std::map<Acc_Axis_T, int32_t> physical_accelerations_;
    std::map<Angle_Axis_T, int32_t> spirit_angles_;
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
    int32_t CalculateAngle(int32_t acc) const;
public:
    Mpu6050AngleConverter(SensorData& data): data_(data){};
    void CalculateSpiritAngles(void);
};


/**
 * @brief Main class of MPU6050, to be used outside module.
 * @param i2c_handle Pointer to the i2c-bus class.
 */
class Mpu6050
{
private:
    I2c* i2c_handle_;
    SensorData sensor_data_;

    Mpu6050Sensor sensor_;
    Mpu6050AccConverter acc_converter_;
    Mpu6050AngleConverter angle_converter_;

public:
    Mpu6050(I2c* i2c_handle);
    void Init(void);
    void Start(void);
    bool HasValidI2cInstance(void) const;
    int32_t GetSpiritAngle(Angle_Axis_T axis) const;
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    void ReadAndProcessSensorData(void);

    /*UT interfaces:*/
#ifdef _UNIT_TEST
    void SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc);
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    int16_t GetRawAcceleration (Acc_Axis_T axis) const;
    void ReadSensorData(void);
    void ConvertRawToPhysical(void);
    void CalculateSpiritAngles(void);
#endif /*_UNIT_TEST*/
};
