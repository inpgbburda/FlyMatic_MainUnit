#include "mpu6050.hpp"
#include "i2c.hpp"

#include <vector>
#include <stdexcept>
#include <math.h>
#include <map>

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

#define DEGREES_IN_RADIAN 57.295779513f
#define ONE_G_TRESHOLD 1000

I2c i2c_bus;

static pthread_mutex_t Angle_Lock;
static pthread_mutex_t Acc_Lock;

static const std::map<Acc_Axis_T, uint8_t> Acc_Reg_Cfg
{
    {X, ACCEL_XOUT_H},
    {Y, ACCEL_YOUT_H},
    {Z, ACCEL_ZOUT_H},
};

static const std::map<Acc_Axis_T, Acc_Axis_T> Spirits_Cfg
{
    {X, Y},
    {Y, X},
};


Mpu6050::Mpu6050():i2c_handle_(nullptr)
{
}

void Mpu6050::Init(I2c* i2c_ptr)
{
    pthread_mutex_init(&Angle_Lock, NULL);
    pthread_mutex_init(&Acc_Lock, NULL);
    i2c_handle_ = i2c_ptr;
}

void Mpu6050::Start(void)
{
    bool sensor_detected = false;
    sensor_detected = CheckSensorPresence();
    if(sensor_detected)
    {
        i2c_handle_ -> WriteByte(MPU6050_I2C_ADDR, PWR_MGMT_1, PWR_MGMT_1_WAKE_UP);
    }
}

void Mpu6050::MainFunc(void)
{
    Acc_Axis_T axis = MAX_AXIS_NUMBER;
    int16_t raw_acc = 0;

    for(int i=0; i<MAX_AXIS_NUMBER; i++)
    {
        axis = static_cast<Acc_Axis_T>(i);
        raw_acc = ReadAccceleration(axis);
        SetRawAcceleration(axis, raw_acc);
    }
    ConvertAccelerations();
    CalculateSpiritAngles();
}

bool Mpu6050::CheckSensorPresence(void) const
{
    int who_i_am = i2c_handle_->ReadByte(MPU6050_I2C_ADDR, WHO_AM_I);
    return WHO_AM_I_VAL == who_i_am;
}


int16_t Mpu6050::ReadAccceleration(Acc_Axis_T axis) const
{
    std::vector<uint8_t> Acc_Bytes;
    int16_t acc = 0;
    uint8_t start_reg = 0x00;

    start_reg = Acc_Reg_Cfg.at(axis);
    Acc_Bytes = i2c_handle_->ReadBlockOfBytes(MPU6050_I2C_ADDR, start_reg, ACC_SIZE);

    acc = ((int16_t)(Acc_Bytes[ACC_H]) << 8) + (int16_t)Acc_Bytes[ACC_L];

    return acc;
}

void Mpu6050::SetRawAcceleration(Acc_Axis_T axis, int16_t acc)
{
    Raw_Accelerations_[axis] = acc;
}

void Mpu6050::SetPhysicalAcceleration(Acc_Axis_T axis, int32_t acc)
{
    Physical_Accelerations_[axis] = acc;
}

void Mpu6050::CalculateSpiritAngles(void)
{
    int spirits[2] = {0};
    pthread_mutex_lock(&Angle_Lock);
    for(int i=0; i<2; i++){
        if(ONE_G_TRESHOLD < abs(Physical_Accelerations_[i])){
            spirits[i] = 0;
        }
        else{
            spirits[i] = std::round(DEGREES_IN_RADIAN * (-asin((float)Physical_Accelerations_[i]/1000.0)));
        }
    }
    Spirit_Angle_Y_ = spirits[0];
    Spirit_Angle_X_ = spirits[1];
    pthread_mutex_unlock(&Angle_Lock);
}

int32_t Mpu6050::GetSpiritAngle(Acc_Axis_T axis) const
{
    int32_t angle = 0;
    
    pthread_mutex_lock(&Angle_Lock);
    if(X == axis){
        angle = Spirit_Angle_X_;
    }
    else if(Y == axis){
        angle = Spirit_Angle_Y_;
    }
    else{
        /*Do nothing*/
    }
    pthread_mutex_unlock(&Angle_Lock);
    return angle;
}

int32_t Mpu6050::GetPhysicalAcceleration(Acc_Axis_T axis) const
{
    int32_t acc = 0;
    pthread_mutex_lock(&Acc_Lock);
    acc = Physical_Accelerations_[axis];
    pthread_mutex_unlock(&Acc_Lock);
    return acc;
}

bool Mpu6050::HasValidI2cInstance(void) const
{
    return nullptr != i2c_handle_;
}

void Mpu6050::ConvertAccelerations(void)
{
    pthread_mutex_lock(&Acc_Lock);
    for(int i=0; i<MAX_AXIS_NUMBER; i++)
    {
        int32_t raw_value = Raw_Accelerations_[i];
        Physical_Accelerations_[i] = raw_value * ACC_MAX_VAL * 1000 / INT16_T_MAX_VAL;
    }
    pthread_mutex_unlock(&Acc_Lock);
}

Mpu6050::~Mpu6050()
{
}
