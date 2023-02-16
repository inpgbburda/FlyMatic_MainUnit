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
#include <string.h>				/* Needed for memset() call*/
#include <errno.h>

#include "Thread_Manager.hpp"

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

extern uint32_t Time_Calibration_G;
pthread_mutex_t Pwm_lock_G;

int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags) 
{
    return syscall(__NR_sched_setattr, pid, attr, flags);
}


void *Do_Pwm(void *data_ptr)
{
    int flags = 0;
    int ret;
    const struct sched_attr* attr_ptr = (const struct sched_attr*) data_ptr;
    struct sched_attr attr_local = *attr_ptr;
    memset(&attr_local, 0, sizeof(attr_local)); 
    ret = sched_setattr(0, &attr_local, flags);
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


void *Do_Main_Routine(void *data)
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


void *calculate_flight_controls(void *data_ptr)
{
    int ret;
    int flags = 0;
    ret = sched_setattr(0, (const struct sched_attr*)data_ptr, flags);
    
    if (ret < 0) {
        perror("sched_setattr failed to set the priorities");
        exit(-1);
    };
    std::cout << "Started flight controls"<< std::endl;
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

    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;
    Init_Pwm();
    RT_Thread rt_thread_1 = RT_Thread(SCHED_DEADLINE, 100, 1000, 10000, &Do_Pwm, Core_Set_1);
    rt_thread_1.Init();
    rt_thread_1.Run();
    rt_thread_1.Assign_Affinity();
    // RT_Thread rt_thread_2 = RT_Thread(SCHED_DEADLINE, 100, 1000, 10000, &calculate_flight_controls, Core_Set_2);
    // rt_thread_2.Init();
    // rt_thread_2.Run();
    // rt_thread_2.Assign_Affinity();

    return 0;
}
