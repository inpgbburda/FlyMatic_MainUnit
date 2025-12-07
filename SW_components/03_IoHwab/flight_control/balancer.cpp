/**
* File contains 
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/
#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "mpu6050.hpp"
#include "spi.hpp"

#include <iostream>
#include <cmath>
/*
|===================================================================================================================================|
    Macro definitions
|===================================================================================================================================|
*/

/*
|===================================================================================================================================|
    Local types definitions 
|===================================================================================================================================|
*/
/*
|===================================================================================================================================|
    Object allocations 
|===================================================================================================================================|
*/
extern Mpu6050 mpu6050;
Spi Spi_Bus;

const int base = 25;
const float k = 0.3f;
const float I = 0.02f;
const float target_angle = -20;

static const int SPI_CHANNEL = 0;
static const int SPI_SPEED = 500000;

/*
|===================================================================================================================================|
    Local function declarations
|===================================================================================================================================|
*/

/*
|===================================================================================================================================|
    Function definitions
|===================================================================================================================================|
*/

void *CalculateFlightControls(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    uint8_t buffer[MAX_MOTOR_NUM] = {0};
    std::cout << "Step 1" << std::endl;
    buffer[0] = 0x0;
    buffer[1] = 0x0;
    buffer[2] = 0x0;
    buffer[3] = 0x0;
    Spi_Bus.Init(SPI_CHANNEL, SPI_SPEED);

    Spi_Bus.ReadWriteData(SPI_CHANNEL, buffer, 4);
    Balancer *balancer = new Balancer();
    
    sleep(5);
    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(k, I, 0);
    balancer->SetBaseThrust(30);

    while(1)
    {
        balancer->ProcessControl();
        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return NULL;
}

void *ReadAccSensor(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    while(1)
    {
        mpu6050.ReadSensorData();
        sched_yield();
    }
    return NULL;
}

void *DoMainRoutine(void)
{
    sleep(60);
    return NULL;
}

Balancer::Balancer(/* args */)
{
}

void Balancer::SetBaseThrust(uint8_t thrust)
{
    base_thrust_ = thrust;
    thrust_1_ = thrust;
    thrust_2_ = thrust;
}

uint8_t Balancer::GetCurrentThrust(Motor_Id_T channel) const
{
    uint8_t thr = 0;

    if(MOTOR_1 == channel){
        thr = thrust_1_;
    }
    else if(MOTOR_2 == channel){
        thr = thrust_2_;
    }
    else{
        thr = 0;
    }
    return thr;
}

Balancer::~Balancer()
{
}

void Balancer::ProcessControl(void)
{
    uint8_t spi_buffer[MAX_MOTOR_NUM] = {0};

    int32_t roll_angle = 0;
    float u = 0.0f;
    float error = 0.0f;
    float error_d = 0.0f;
    int32_t thrust_1 = 0;
    int32_t thrust_2 = 0;

    mpu6050.ProcessSensorData();
    roll_angle = mpu6050.GetSpiritAngle(ROLL);

    error = target_angle_ - static_cast<float>(roll_angle);
    error_i_ = error + error_i_;
    error_d = error - error_prev_;

    u =  kp_*error + ki_*error_i_ + kd_*error_d;

    float rounded_u = std::round(u);
    int32_t temp_u = static_cast<int32_t>(rounded_u);

    thrust_1 = static_cast<int32_t>(base_thrust_) + temp_u;
    thrust_2 = static_cast<int32_t>(base_thrust_) - temp_u;
    
    thrust_1_ = (thrust_1 > 0) ? static_cast<uint8_t>(thrust_1) : 0;
    thrust_2_ = (thrust_2 > 0) ? static_cast<uint8_t>(thrust_2) : 0;

#ifndef _UNIT_TEST
    std::cout << " roll angle X: " << roll_angle <<"; Power 1 " << thrust_1_ << "; Power 2 " << thrust_2_ << "; error " << error<< std::endl;
#endif

    spi_buffer[MOTOR_1] = thrust_1_;
    spi_buffer[MOTOR_2] = thrust_2_;

    Spi_Bus.ReadWriteData(SPI_CHANNEL, spi_buffer, sizeof(spi_buffer));
}

void Balancer::SetTargetAngle(int32_t angle)
{
    target_angle_ = angle;
}

void Balancer::SetRegulatorConstants(float kp, float ki, float kd)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

