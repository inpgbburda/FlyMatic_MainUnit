#pragma once

#include <pthread.h>
#include <stdint.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */
#include <unistd.h>           /* Definition of syscalls */
#include <iostream>
#include <vector>

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

typedef struct
{
    sched_attr_t* attr_ptr; // Thread scheduling attributes
    void* user_arg;    // User argument passed to thread function
}
RT_Thread_StartPayload;

void SchedSetAttr(sched_attr_t *attr_ptr);
void PreventPagingToSwapArea(void);

class RT_Thread
{

private:
    /* data */
    pthread_t posix_instance_;
    void* (*fun_ptr_)(void *data);
    sched_attr_t attr_;
    bool Cpu_Set_[THR_MNGR_RPI_CORE_NUMBER];
    bool exec_state_;
    RT_Thread_StartPayload start_payload_;

public:
    RT_Thread
    (
        int scheduler_type, int runtime, int deadline, int period, void* (*fun_ptr)(void *data)
    );
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

   ~Thread_Manager(){};
};
