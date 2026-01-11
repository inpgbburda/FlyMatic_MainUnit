#pragma once

/**
* Manages real-time threads creation and scheduling
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/

#include <pthread.h>
#include <stdint.h>
#ifndef _UNIT_TEST
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>           /* Definition of syscalls */
#endif
#include <iostream>
#include <vector>
/*
|===================================================================================================================================|
    Macro definitions
|===================================================================================================================================|
*/
#define THR_MNGR_RPI_CORE_NUMBER 4U
#define SCHED_US_MULTP 10U /* To obtain 1 us timestamp, it's required by scheduler API to pass value 10*/

#define handle_error_en(en, msg) \
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

/*
|===================================================================================================================================|
    Exported types declarations
|===================================================================================================================================|
*/

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
SchedAttr_T;

typedef struct
{
    SchedAttr_T* attr_ptr; // Thread scheduling attributes
    void* user_arg;    // User argument passed to thread function
}
RT_Thread_StartPayload;

void SchedSetAttr(SchedAttr_T *attr_ptr);
void PreventPagingToSwapArea(void);

class RT_Thread
{

private:
    /* data */
    pthread_t posix_instance_;
    void* (*fun_ptr_)(void *data);
    SchedAttr_T attr_;
    bool cpu_set_[THR_MNGR_RPI_CORE_NUMBER];
    bool exec_state_;
    RT_Thread_StartPayload start_payload_;

public:
    RT_Thread
    (
        int scheduler_type, int runtime, int deadline, int period, void* (*fun_ptr)(void *data)
    );
    RT_Thread(const RT_Thread& other);
    RT_Thread& operator=(const RT_Thread& other);
    void SetUserArg(void* arg);
    /** Creates Posix thread with prevoiusly set paramaters*/
    void Run(void);
    bool IsRun(void) {return exec_state_;}
    void AssignAffinity(void);
    void Join(void) { pthread_join(posix_instance_, NULL); }
    bool operator==(const RT_Thread& rt_thread)const;

   ~RT_Thread(){};
};


class Thread_Manager
{
private:
   /* data */
   std::vector<RT_Thread> collected_threads_;

public:
    Thread_Manager(/* args */){};

    void Init(std::vector<RT_Thread> &thread_list){ collected_threads_ = thread_list; }
    void AddThread(RT_Thread &thread) {};
    std::vector<RT_Thread> const GetAllThreads(){ return collected_threads_; }
    void DeInit(void);
    void RunAllThreads(void);
    ~Thread_Manager();
};
