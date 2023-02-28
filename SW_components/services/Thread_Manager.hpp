#pragma once

#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>           /* Definition of syscalls */
#include <iostream>

#define THR_MNGR_RPI_CORE_NUMBER 4U
#define SCHED_US_MULTP 10U /* To obtain 1 us timestamp, it's required by scheduler API to pass value 10*/

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct
{
    uint32_t size;
    uint32_t sched_policy;
    uint64_t sched_flags;
    int32_t sched_nice;
    uint32_t sched_priority;
    uint64_t sched_runtime;
    uint64_t sched_deadline;
    uint64_t sched_period;
}
sched_attr_t;


class Thread_Manager
{
private:
   /* data */
public:
    Thread_Manager(/* args */);
    void Init(void);
    void DeInit(void);

   ~Thread_Manager();
};


class RT_Thread
{
private:
    /* data */
    int scheduler_type_;
    int runtime_;
    int deadline_;
    int period_;
    pthread_t posix_instance_;
    void* (*fun_ptr_)(void *data);
    sched_attr_t attr_;
    bool Cpu_Set_[THR_MNGR_RPI_CORE_NUMBER];

public:
    RT_Thread
            (
                int scheduler_type, int runtime, int deadline, int period, void* (*fun_ptr)(void *data), bool (&cpu_set)[THR_MNGR_RPI_CORE_NUMBER]
            )
    {
        scheduler_type_ = scheduler_type;
        runtime_ = runtime;
        deadline_ = deadline;
        period_ = period;
        fun_ptr_ = fun_ptr;
        for(unsigned int i=0; i<THR_MNGR_RPI_CORE_NUMBER; i++)
        {
            Cpu_Set_[i] = cpu_set[i];
        }
    }

    /** Copies thread parameters into standarised attributes structure.
    */
    void Init(void){
        attr_.size = sizeof(attr_);
        attr_.sched_policy = scheduler_type_;
        attr_.sched_runtime = runtime_;
        attr_.sched_deadline = deadline_;
        attr_.sched_period = period_;
    }

    /** Creates Posix thread with prevoiusly set paramaters
    */
    void Run(void){
        pthread_create(&posix_instance_, NULL, fun_ptr_, (void*)&attr_);
    }

    void AssignAffinity(void)
    {
        int aff_result;
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        for(unsigned int i=0; i<THR_MNGR_RPI_CORE_NUMBER; i++)
        {
            if(Cpu_Set_[i])
            {
                CPU_SET(i, &cpuset);
            }
        } 
        aff_result = pthread_setaffinity_np(posix_instance_,sizeof(cpuset), &cpuset);
        if (0 != aff_result)
        {
            std::cout << "Error- affinity problem" << std::endl;
        }
    }

    void Join(void)
    {
        pthread_join(posix_instance_, NULL);
    }

    void DeInit(void);

   ~RT_Thread(){};
};


int SchedSetAttr(pid_t pid, sched_attr_t *attr_ptr,unsigned int flags);
