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
const int base = 25;
const float k = 0.2f;
const float I = 0.08f;
const float D = 0.0f;
const float target_angle = -20;

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
    RT_Thread_StartPayload *payload = static_cast<RT_Thread_StartPayload*>(data_ptr); 

    SchedSetAttr(payload->attr_ptr);
    Balancer* balancer = static_cast<Balancer*>(payload->user_arg);

    std::cout << "Step 1" << std::endl;
    balancer->Init();
    sleep(5);
    balancer->SetRegulatorConstants(k, I, D);
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
    RT_Thread_StartPayload *payload = static_cast<RT_Thread_StartPayload*>(data_ptr);
    
    SchedSetAttr(payload->attr_ptr);
    Mpu6050* mpu6050 = static_cast<Mpu6050*>(payload->user_arg);

    while(1)
    {
        mpu6050->ReadSensorData();
        sched_yield();
    }
    return NULL;
}

void *DoMainRoutine(Balancer& balancer)
{
    balancer.SetTargetAngle(0);
    sleep(10);
    balancer.SetTargetAngle(20);
    sleep(5);
    balancer.SetTargetAngle(-20);
    sleep(5);
    balancer.SetTargetAngle(20);
    sleep(5);
    balancer.SetTargetAngle(-20);
    sleep(5);
    balancer.SetTargetAngle(0);
    sleep(10);
    return NULL;
}

Balancer::Balancer(Mpu6050& mpu6050, Spi& spi, int spi_channel):
    mpu6050_(mpu6050), spi_(spi), spi_channel_(spi_channel)
{
}

void Balancer::Init(void)
{
    uint8_t buffer[MAX_MOTOR_NUM] = {0};
    spi_.ReadWriteData(spi_channel_, buffer, MAX_MOTOR_NUM);
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


void Balancer::ProcessControl(void)
{
    uint8_t spi_buffer[MAX_MOTOR_NUM] = {0};

    int32_t roll_angle = 0;
    float u = 0.0f;
    float error = 0.0f;
    float error_d = 0.0f;
    int32_t thrust_1 = 0;
    int32_t thrust_2 = 0;

    mpu6050_.ProcessSensorData();
    roll_angle = mpu6050_.GetSpiritAngle(ROLL);

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

    spi_.ReadWriteData(spi_channel_, spi_buffer, sizeof(spi_buffer));
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

Balancer::~Balancer()
{
}
