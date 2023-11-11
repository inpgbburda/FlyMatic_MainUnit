#include "mpu6050.hpp"
#include "i2c.hpp"

#include <stdexcept>
#include <math.h>
#include <vector>

#define MPU6050_I2C_ADDR 0x68U

/* MPU6050 hardware register adresses */
#define WHO_AM_I 0x75U
#define WHO_AM_I_VAL 0x68U

#define PWR_MGMT_1 0x6BU
#define PWR_MGMT_1_WAKE_UP 0x00U

#define ACCEL_XOUT_H 0x3BU
#define ACCEL_YOUT_H 0x3DU
#define ACCEL_ZOUT_H 0x3FU

/* Auxiliary constants */
#define ACC_H 0U
#define ACC_L 1U
#define ACC_SIZE 2U

#define INT16_T_MAX_VAL 32767
#define ACC_MAX_VAL 2

/* Spirit angle calculation constants */
#define DEGREES_IN_RADIAN   57.295779513f
#define ONE_G_TRESHOLD      1000
#define ONE_G               1000.0f

I2c i2c_bus;

static pthread_mutex_t Angle_Lock;
static pthread_mutex_t Acc_Lock;

static const std::map<Acc_Axis_T, uint8_t> Acc_Reg_Cfg
{
    {X, ACCEL_XOUT_H},
    {Y, ACCEL_YOUT_H},
    {Z, ACCEL_ZOUT_H},
};

static const std::map<Angle_Axis_T, Acc_Axis_T> Angle_Cfg
{
    {ROLL , Y},
    {PITCH, X}
};

Mpu6050::Mpu6050(I2c *i2c_handle): 
    i2c_handle_(i2c_handle), sensorData_(), sensor_(i2c_handle, sensorData_),
    accConverter_(sensorData_), angleConverter_(sensorData_) 
{
    sensorData_.rawAccelerations_=
    {
        {X, 0},
        {Y, 0},
        {Z, 0}
    };
    sensorData_.physicalAccelerations_=
    {
        {X, 0},
        {Y, 0},
        {Z, 0}
    };
    sensorData_.spiritAngles_=
    {
        {ROLL, 0},
        {PITCH, 0}
    };
};

void Mpu6050::Init(void)
{
    pthread_mutex_init(&Angle_Lock, NULL);
    pthread_mutex_init(&Acc_Lock, NULL);
}

void Mpu6050::Start(void)
{
    sensor_.Start();
}

int32_t Mpu6050::GetSpiritAngle(Angle_Axis_T axis) const
{
    int32_t angle = 0;
    
    pthread_mutex_lock(&Angle_Lock);
    angle = sensorData_.spiritAngles_.at(axis);
    pthread_mutex_unlock(&Angle_Lock);
    return angle;
}

int32_t Mpu6050::GetPhysicalAcceleration(Acc_Axis_T axis) const
{
    int32_t acc = 0;
    pthread_mutex_lock(&Acc_Lock);
    acc = sensorData_.physicalAccelerations_.at(axis);
    pthread_mutex_unlock(&Acc_Lock);
    return acc;
}

void Mpu6050::ReadAndProcessSensorData(void)
{
     {
        /* Read sensor data, update shared SensorData, and perform conversions and calculations */
        sensor_.ReadSensorData();
        accConverter_.ConvertRawToPhysical();
        angleConverter_.CalculateSpiritAngles();
    }
}

bool Mpu6050::HasValidI2cInstance(void) const
{
    return nullptr != i2c_handle_;
}

void Mpu6050AngleConverter::CalculateSpiritAngles(void)
{
    int   angle = 0;
    int   acc   = 0;
    float acc_f    = 0.0;
    float angle_f  = 0.0;

    pthread_mutex_lock(&Angle_Lock);

    for (auto &x : data_.spiritAngles_)
    {
        Acc_Axis_T assigned_axis = Angle_Cfg.at(x.first);
        acc = data_.physicalAccelerations_.at(assigned_axis);

        if(ONE_G_TRESHOLD >= abs(acc)){
            acc_f = (float)acc;
            angle_f = DEGREES_IN_RADIAN * (-asin(acc_f/ONE_G));
            angle = std::round(angle_f);
        }
        else{
            angle = 0;
        }
        x.second = angle;
    }

    pthread_mutex_unlock(&Angle_Lock);
}

void Mpu6050Sensor::ReadSensorData(void)
{
    int16_t raw_acc = 0;

    for(auto &x : data_to_fill_.rawAccelerations_)
    {
        raw_acc = ReadAcceleration(x.first);
        x.second = raw_acc;
    }
}

bool Mpu6050Sensor::CheckPresence(void) const
{
    int who_i_am = i2c_handle_->ReadByte(MPU6050_I2C_ADDR, WHO_AM_I);
    return WHO_AM_I_VAL == who_i_am;
}

int16_t Mpu6050Sensor::ReadAcceleration(Acc_Axis_T axis) const
{
    std::vector<uint8_t> Acc_Bytes;
    int16_t acc = 0;
    uint8_t start_reg = 0x00;

    start_reg = Acc_Reg_Cfg.at(axis);
    Acc_Bytes = i2c_handle_->ReadBlockOfBytes(MPU6050_I2C_ADDR, start_reg, ACC_SIZE);

    acc = ((int16_t)(Acc_Bytes[ACC_H]) << 8) + (int16_t)Acc_Bytes[ACC_L];

    return acc;
}

void Mpu6050Sensor::Start(void)
{
    bool sensor_detected = false;
    sensor_detected = CheckPresence();
    if(sensor_detected)
    {
        i2c_handle_ -> WriteByte(MPU6050_I2C_ADDR, PWR_MGMT_1, PWR_MGMT_1_WAKE_UP);
    }
}

void Mpu6050AccConverter::ConvertRawToPhysical(void)
{
    std::map<Acc_Axis_T, int32_t>& rawAccs = data_.rawAccelerations_;
    std::map<Acc_Axis_T, int32_t>& physAccs = data_.physicalAccelerations_;

    pthread_mutex_lock(&Acc_Lock);
    for(auto &x : physAccs)
    {
        int32_t raw_value = rawAccs.at(x.first);
        x.second = raw_value * ACC_MAX_VAL * 1000 / INT16_T_MAX_VAL;
    }
    pthread_mutex_unlock(&Acc_Lock);
}

/*UT interfaces:*/
void Mpu6050::SetRawAcceleration(Acc_Axis_T axis, int16_t acc)
{
    sensorData_.rawAccelerations_.at(axis) = acc;
}

int16_t Mpu6050::GetRawAcceleration(Acc_Axis_T axis) const
{
    return sensorData_.rawAccelerations_.at(axis);
}

void Mpu6050::SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc)
{
    sensorData_.physicalAccelerations_.at(axis) = acc;
}

void Mpu6050::ReadSensorData(void)
{
    sensor_.ReadSensorData();
}

void Mpu6050::ConvertRawToPhysical(void)
{
    accConverter_.ConvertRawToPhysical();
}

void Mpu6050::CalculateSpiritAngles(void)
{
    angleConverter_.CalculateSpiritAngles();
}