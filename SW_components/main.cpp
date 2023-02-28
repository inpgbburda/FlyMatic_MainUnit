#include <iostream>
#include "pwm.hpp"
#include <mutex>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/mman.h>
#include <linux/sched.h>      /* Definition of SCHED_* constants */
#include <sys/syscall.h>      /* Definition of SYS_* constants */

#include "Thread_Manager.hpp"
#include <wiringPi.h>


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

extern uint32_t Time_Calibration_G;
pthread_mutex_t Pwm_lock_G;


void *DoPwm(void *data_ptr)
{
    int flags = 0;
    int ret;
    ret = SchedSetAttr(0, (sched_attr_t*)data_ptr, flags);
    std::cout << ret << std::endl;
    if (ret < 0) {
        perror("sched_setattr failed to set the priorities");
        exit(-1);
    };
    std::cout << "Started pwm routine"<< std::endl;
    while(1){
        Run_Pwm_Blocking();
    }
}


void *DoMainRoutine(void *data)
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


void *CalculateFlightControls(void *data_ptr)
{
    int ret;
    int flags = 0;
    ret = SchedSetAttr(0, (sched_attr_t*)data_ptr, flags);
    if (ret < 0) {
        perror("sched_setattr failed to set the Flight Controls priorities");
        exit(-1);
    };
    volatile int cnt = 0;
    /*Just for debugg - simulate high load task*/
    while(1)
    {   
        digitalWrite (1, true);
        for(cnt=0; cnt<100; cnt++);
        digitalWrite (1, false);
    }
  
    return NULL;
}


int main()
{
    /* Lock memory - prevent from paging to the swap area -
    * all of the process memory will stay in RAM
    */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) 
    {
        printf("mlockall failed: %m\n");
        exit(-2);
    }

    bool Core_Set_1[4] = {true, false, false, false};
    bool Core_Set_2[4] = {false, true, true, true};
    bool Core_Set_3[4] = {false, true, true, true};

    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;
    Init_Pwm();
    RT_Thread rt_thread_1 = RT_Thread(SCHED_DEADLINE, 100000, 100000, 100000, &DoPwm, Core_Set_2);
    rt_thread_1.Init();
    rt_thread_1.Run();

    RT_Thread rt_thread_2 = RT_Thread(SCHED_DEADLINE, 100000, 100000, 20000000, &CalculateFlightControls, Core_Set_2);
    rt_thread_2.Init();
    rt_thread_2.Run();
    DoMainRoutine(NULL);

    return 0;
}
