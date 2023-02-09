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


void *Do_Pwm(void *data)
{
    int s;
   
    pthread_t thread;
    cpu_set_t cpuset;
    thread = pthread_self();/**returns the ID of the thread in which it is invoked*/
    CPU_ZERO(&cpuset); 
    CPU_SET(1, &cpuset);
     
    /**Assign CPU to current thread*/
   s = pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
   if (s != 0)
      handle_error_en(s, "pthread_setaffinity_np");
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


struct sched_attr {
    uint32_t size;
    uint32_t sched_policy;
    uint64_t sched_flags;
    int32_t sched_nice;
    uint32_t sched_priority;
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
};

int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags) 
{
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

void *calculate_flight_controls(void *data_ptr)
{
    int ret;
    int flags = 0;
    struct sched_attr attr;
    cpu_set_t cpuset;
    pthread_t thread;
    thread = pthread_self();/**returns the ID of the thread in which it is invoked*/
    CPU_SET(2, &cpuset);
    /**Assign CPU to current thread*/
    pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
    memset(&attr, 0, sizeof(attr)); 
    attr.size = sizeof(attr);

    ret = sched_setattr(0, (const struct sched_attr*)data_ptr, flags);
    
    if (ret < 0) {
        perror("sched_setattr failed to set the priorities");
        exit(-1);
    };
    return NULL;
}


int main()
{
    pthread_t thread_1;
    pthread_t thread_2;
    pthread_t thread_3;
    struct sched_attr attr_1;
    attr_1.sched_policy   = SCHED_DEADLINE;
    attr_1.sched_runtime  =  100000;
    attr_1.sched_deadline = attr_1.sched_period = 1000000;

    /* Lock memory - prevent from paging to the swap area -
    * all of the process memory will stay in RAM
    */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) 
    {
        printf("mlockall failed: %m\n");
        exit(-2);
    }

    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;
    RT_Thread rt_thread_1 = RT_Thread(SCHED_DEADLINE, 1000, 10000, 1000, &calculate_flight_controls);
    Init_Pwm();
    pthread_create(&thread_1, NULL, calculate_flight_controls, &attr_1);
    pthread_create(&thread_2, NULL, Do_Pwm, NULL);
    pthread_create(&thread_3, NULL, Do_Main_Routine, NULL);

    pthread_join(thread_3, NULL);
    return 0;
}
