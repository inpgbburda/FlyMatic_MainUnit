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

    int x_phys_acc = 0;
    
    while(1)
    {
        mpu6050.MainFunc();
        x_phys_acc = mpu6050.GetPhysicalAcceleration(X);
        std::cout << "Acceleration is: "<< x_phys_acc << std::endl;

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
        sleep(5);

        std::cout << "Step 2" << std::endl;
        Set_Pwm(CHAN_1, 25U);
        sleep(5);

        std::cout << "Step 3" << std::endl;
        Set_Pwm(CHAN_1, 50U);
        sleep(5);

        std::cout << "Step 4" << std::endl;
        Set_Pwm(CHAN_1, 100U);
        sleep(5);
        pthread_mutex_destroy(&Pwm_lock_G);
    return NULL;
}
