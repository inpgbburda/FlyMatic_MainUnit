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
#include "pwm.hpp"
#include "mpu6050.hpp"

#include <iostream>
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
pthread_mutex_t Pwm_lock_G;

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

const int base = 25;
const float k = 0.03;
const float I = 0.02;

void *CalculateFlightControls(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);

    int32_t angle_x = 0;
    int diff=0;
    sleep(5);
    static float prev_diff = 0;

    while(1)
    {
        int power_1;
        int power_2;
        int control;
        mpu6050.ReadAndProcessSensorData();
        angle_x = mpu6050.GetSpiritAngle(ROLL);
        diff = 0 - angle_x;
        prev_diff = prev_diff + (float)diff;
        control  = int((float)diff*(-k) + prev_diff*(-I));
        power_1 = base + control;
        power_2 = base - control;
        std::cout << " roll angle X: " << angle_x <<"; Power 1 " << power_1 << "; Power 2 " << power_2 << std::endl;
        Set_Pwm(CHAN_1, power_1);
        Set_Pwm(CHAN_2, power_2);
        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return NULL;
}

void *DoMainRoutine(void)
{
        Pwm_lock_G = PTHREAD_MUTEX_INITIALIZER;
        std::cout << "Step 1" << std::endl;
        Set_Pwm(CHAN_1, 0U);
        Set_Pwm(CHAN_2, 0U);
        sleep(3);

        sleep(30);
        pthread_mutex_destroy(&Pwm_lock_G);
    return NULL;
}

Balancer::Balancer(/* args */)
{
}

void Balancer::SetBaseThrust(int32_t thrust)
{
    thrust_ = thrust;
}

int32_t Balancer::GetCurrentThrust(Motor_Id_T channel) const
{
    return thrust_;
}

Balancer::~Balancer()
{
}

void Balancer::ProcessControl(void) const
{
    Set_Pwm(CHAN_1, 25);
    Set_Pwm(CHAN_2, 25);
}



