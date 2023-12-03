#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "pwm.hpp"
#include "mpu6050.hpp"

#include <iostream>

extern uint32_t Time_Calibration_G;
extern Mpu6050 mpu6050;
pthread_mutex_t Pwm_lock_G;

void *CalculateFlightControls(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);

    int32_t angle_x = 0;
    
    while(1)
    {
        mpu6050.ReadAndProcessSensorData();
        angle_x = mpu6050.GetSpiritAngle(ROLL);
        std::cout << " roll angle X: " << angle_x << std::endl;

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
        sleep(5);

        std::cout << "Step 2" << std::endl;
        Set_Pwm(CHAN_1, 25U);
        Set_Pwm(CHAN_2, 25U);
        sleep(5);

        std::cout << "Step 3" << std::endl;
        Set_Pwm(CHAN_1, 50U);
        Set_Pwm(CHAN_2, 50U);
        sleep(5);

        std::cout << "Step 4" << std::endl;
        Set_Pwm(CHAN_1, 70U);
        Set_Pwm(CHAN_2, 70U);
        sleep(5);
        pthread_mutex_destroy(&Pwm_lock_G);
    return NULL;
}

Balancer::Balancer(/* args */)
{
}

Balancer::~Balancer()
{
}

void Balancer::Runable(void) const
{
}
