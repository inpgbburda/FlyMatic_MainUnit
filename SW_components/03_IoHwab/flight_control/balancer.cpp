/**
* File contains implementation of balancing control algorithm
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/
#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "Monotonic_Sleep.hpp"
#include "mpu6050.hpp"
#include "spi.hpp"

#include <iostream>
#include <cmath>

/*
|===================================================================================================================================|
    Macro definitions
|===================================================================================================================================|
*/
#define SLOW_SHUTDOWN_STEP_DELAY_US 200000U

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
const float k = 0.12f;
const float I = 0.1f;
const float D = 0.25f;

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

void *CalculateFlightControlsLoop(SchedAttr_T* /*attr*/, FlightCtrlArgs_T* args)
{
    auto* balancer = static_cast<Balancer*>(args->balancer);

    std::cout << "Step 1" << std::endl;
    balancer->Init();
    SleepMonotonicRawMs(5000U);
    balancer->SetRegulatorConstants(k, I, D);
    balancer->SetBaseThrust(30);

    while(!args->stop->load(std::memory_order_relaxed)){
        balancer->ProcessControl();
        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return nullptr;
}

void *ReadAccSensorLoop(SchedAttr_T* /*attr*/, ReadAccSensorArgs_T* args)
{
    auto* mpu6050 = static_cast<Mpu6050*>(args->mpu6050);

    while(!args->stop->load(std::memory_order_relaxed)){
        mpu6050->ReadSensorData();
        sched_yield();
    }
    return nullptr;
}

void *DoMainRoutine(Balancer& balancer)
{
    balancer.SetTargetAngle(0);
    SleepMonotonicRawMs(10000U);
    balancer.SetTargetAngle(15);
    SleepMonotonicRawMs(5000U);
    balancer.SetTargetAngle(-15);
    SleepMonotonicRawMs(5000U);
    balancer.SetTargetAngle(15);
    SleepMonotonicRawMs(5000U);
    balancer.SetTargetAngle(-15);
    SleepMonotonicRawMs(5000U);
    balancer.SetTargetAngle(0);
    SleepMonotonicRawMs(3000U);

    /* Perform slow shutdown of motors */
    balancer.SetBaseThrust(25);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);

    balancer.SetBaseThrust(20);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);

    balancer.SetBaseThrust(15);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);
    
    balancer.SetBaseThrust(10);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);

    balancer.SetBaseThrust(5);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);
    
    balancer.SetBaseThrust(0);
    SleepMonotonicRawUs(SLOW_SHUTDOWN_STEP_DELAY_US);

    return nullptr;
}

Balancer::Balancer(Mpu6050& mpu6050, Spi& spi, int spi_channel):
    mpu6050_(mpu6050), spi_(spi), spi_channel_(spi_channel)
{
}

void Balancer::Init(void)
{
    uint8_t buffer[MAX_MOTOR_NUM] = {0U};
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

    mpu6050_.ProcessSensorData();
    int32_t roll_angle = mpu6050_.GetSpiritAngle(ROLL);
    int32_t target_angle = target_angle_.load(std::memory_order_relaxed);

    float error = target_angle - static_cast<float>(roll_angle);
    error_i_ = error + error_i_;
    float error_d = error - error_prev_;
    error_prev_ = error;

    float u =  kp_*error + ki_*error_i_ + kd_*error_d;

    float rounded_u = std::round(u);
    int32_t temp_u = static_cast<int32_t>(rounded_u);

    int32_t thrust_1 = static_cast<int32_t>(base_thrust_) + temp_u;
    int32_t thrust_2 = static_cast<int32_t>(base_thrust_) - temp_u;
    
    thrust_1_ = (thrust_1 > 0) ? static_cast<uint8_t>(thrust_1) : 0;
    thrust_2_ = (thrust_2 > 0) ? static_cast<uint8_t>(thrust_2) : 0;

#ifndef _UNIT_TEST
        std::cout << " roll angle X: " << roll_angle
        << "; Power 1 " << static_cast<int>(thrust_1_)
        << "; Power 2 " << static_cast<int>(thrust_2_)
        << "; error "   << error
        << std::endl;
#endif

    spi_buffer[MOTOR_1] = thrust_1_;
    spi_buffer[MOTOR_2] = thrust_2_;

    spi_.ReadWriteData(spi_channel_, spi_buffer, sizeof(spi_buffer));
}

void Balancer::SetTargetAngle(int32_t angle)
{
    target_angle_.store(angle, std::memory_order_relaxed);
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
