#pragma once

#include <pthread.h>
#include <stdint.h>

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
    struct sched_attr attr_;

public:
    RT_Thread
            (
                int scheduler_type, int runtime, int deadline, int period, void* (*fun_ptr)(void *data)
            )
    {
        scheduler_type_ = scheduler_type;
        runtime_ = runtime;
        deadline_ = deadline;
        period_ = period;
        fun_ptr_ = fun_ptr;
    }

    /** Copies thread parameters into standarised attributes structure.
    */
    void Init(void){
        attr_.sched_policy = scheduler_type_;
        attr_.sched_runtime = runtime_;
        attr_.sched_deadline = deadline_;
        attr_.sched_period = period_;
    }

    /** Creates Posix thread with prevoiusly set paramaters
    */
    void Run(void){
        pthread_create(&posix_instance_, NULL, fun_ptr_, &attr_);
    }
    void DeInit(void);

   ~RT_Thread(){};
};
