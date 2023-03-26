#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "pwm.hpp"
#ifdef _RASP
#include <wiringPi.h>
#endif /* _RASP */

extern uint32_t Time_Calibration_G;
pthread_mutex_t Pwm_lock_G;

void *CalculateFlightControls(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    /*Just for testing - simulate high load task
    inifnite loop is needed for cyclic task excecution
    */
    while(1)
    {   
        #ifdef _RASP
        volatile int cnt = 0;
        digitalWrite (1, true);
        for(cnt=0; cnt<10000; cnt++);
        digitalWrite (1, false);
        #endif /* _RASP */
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
