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

// class Mpu6050
// {
// private:
//     /* data */
//     I2c* i2c_handle_;
//     std::map<Acc_Axis_T, int32_t> Raw_Accelerations_;
//     std::map<Acc_Axis_T, int32_t> Physical_Accelerations_;
//     std::map<Angle_Axis_T, int32_t> Spirit_Angles_;

// public:
//     Mpu6050(/* args */);
//     void Init(I2c* i2c_ptr);
//     void Start(void);
//     void MainFunc(void);
//     bool CheckSensorPresence(void) const;
//     bool HasValidI2cInstance(void) const;
//     int16_t ReadAccceleration(Acc_Axis_T axis) const;
//     void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
//     void SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc);
//     void CalculateSpiritAngles(void);
//     int32_t GetSpiritAngle(Angle_Axis_T axis) const;
//     void ConvertAccelerations(void);
//     int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
//     ~Mpu6050();
// };

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
public:
    Mpu6050Sensor();
    void readSensorData(void);
    Mpu6050Sensor(I2c* i2c_handle, SensorData& Data_): i2c_handle_(i2c_handle), data_to_fill_(Data_){};
};

class Mpu6050AccConverter
{
private:
    SensorData& data_;

public:
    Mpu6050AccConverter(SensorData& data): data_(data){};
    void convertRawToPhysical(void);
};

class Mpu6050AngleConverter
{
private:
    SensorData& data_;
public:
    Mpu6050AngleConverter(SensorData& data): data_(data){};
    void calculateSpiritAngles(void);
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
    Mpu6050(I2c* i2c_handle)
        : i2c_handle_(i2c_handle), sensorData_(), sensor_(i2c_handle, sensorData_),
          accConverter_(sensorData_), angleConverter_(sensorData_) 
          {
            sensorData_.rawAccelerations_={
                {X, 0},
                {Y, 0},
                {Z, 0}
            };
            sensorData_.physicalAccelerations_={
                {X, 0},
                {Y, 0},
                {Z, 0}
            };
            sensorData_.spiritAngles_={
                {ROLL, 0},
                {PITCH, 0}
            };
          };
    void Init(void);
    void Start(void);
    bool HasValidI2cInstance(void) const;
    int16_t ReadAccceleration(Acc_Axis_T);
    void SetRawAcceleration(Acc_Axis_T axis, int16_t acc);
    void SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc);
    // void CalculateSpiritAngles(void);
    int32_t GetSpiritAngle(Angle_Axis_T axis) const;
    // void ConvertAccelerations(void);
    int32_t GetPhysicalAcceleration(Acc_Axis_T axis) const;
    void readAndProcessSensorData() {
        /* Read sensor data, update shared SensorData, and perform conversions and calculations */
        sensor_.readSensorData();
        accConverter_.convertRawToPhysical();
        angleConverter_.calculateSpiritAngles();
    }

    // Other methods...

};
