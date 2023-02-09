#pragma once

#include <pthread.h>

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
    int timeout_;
    int period_;
    int duty_cycle_;
    pthread_t posix_instance_;
    void* (*fun_ptr_)(void *data);
public:
    RT_Thread
            (
                int scheduler_type, int timeout, int period, int duty_cycle, void* (*fun_ptr)(void *data)
            )
    {
        scheduler_type_ = scheduler_type;
        timeout_ = timeout;
        period_ = period;
        duty_cycle_ = duty_cycle;
        fun_ptr_ = fun_ptr;
    }
    void Init(void);
    void Run(void){
        pthread_create(&posix_instance_, NULL, fun_ptr_, NULL);
    }
    void DeInit(void);

   ~RT_Thread(){};
};
