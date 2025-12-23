#include "Thread_Manager.hpp"
#include <string.h>
#ifndef _UNIT_TEST
#include <sys/mman.h>
#endif

#define DEFAULT_PID         0U /* Apply the scheduling attributes to the current thread*/
#define SCHED_FLAG_DEFAULT       0U /* No special options */
#define SCHED_FLAG_RESET_ON_FORK 1U /* Reset the scheduling attributes to default on fork */
#define SCHED_FLAG_RECLAIM       2U /* Allows reclaiming unused runtime in certain real-time scheduling policies */

/**
 * SchedSetAttr
 * @brief: Passes the scheduling configuration to the OS
 * @param:attr_ptr - pointer to the structure with scheduling attributes
 *
 * @return: none
 * 
 */
void SchedSetAttr(sched_attr_t *attr_ptr) 
{
    sched_attr_t attr_local = {};
    memcpy(&attr_local, attr_ptr, sizeof(sched_attr_t));
#ifndef _UNIT_TEST
    int result = 0;
    /* Pass the scheduling configuration to the OS */
    result = syscall(__NR_sched_setattr, DEFAULT_PID, &attr_local, SCHED_FLAG_DEFAULT);
    if(result < 0)
    {
        std::cout << "sched_setattr failed to set the priorities"<< std::endl;
    }
#else
    (void)attr_local;
#endif
}

/* Lock memory - prevent from paging to the swap area -
    * all of the process memory will stay in RAM
    */
void PreventPagingToSwapArea(void)
{
    #ifndef _UNIT_TEST
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) 
    {
        printf("mlockall failed: %m\n");
        exit(-2);
    }
    #endif
}

RT_Thread::RT_Thread
(
    int scheduler_type, int runtime, int deadline, int period, void* (*fun_ptr)(void *data)
)
{
    fun_ptr_ = fun_ptr;
    exec_state_ = false;

    attr_.size = sizeof(attr_);
    attr_.sched_policy = scheduler_type;
    attr_.sched_runtime = runtime;
    attr_.sched_deadline = deadline;
    attr_.sched_period = period;

    start_payload_.attr_ptr = &attr_;
    start_payload_.user_arg = nullptr;
}

void RT_Thread::SetUserArg(void* arg)
{
    start_payload_.user_arg = arg;
}

/**
 * RT_Thread::Run
 * @brief: Creates Posix thread with prevoiusly set paramaters and starts its execution
 *
 * @return: none
 * 
 */
void RT_Thread::Run(void)
{
    exec_state_ = true;
    pthread_create(&posix_instance_, NULL, fun_ptr_, (void*)&start_payload_);
}

void RT_Thread::AssignAffinity(void)
{
    #ifndef _UNIT_TEST
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
    aff_result = pthread_setaffinity_np(posix_instance_, sizeof(cpuset), &cpuset);
    if (0 != aff_result)
    {
        std::cout << "Error- affinity problem" << std::endl;
    }
    #endif
}

/**
 * RT_Thread:: operator==
 * @brief: Comparison operator for RT_Thread class
 * @param:rt_thread - reference to the RT_Thread object to compare with
 *
 * @return: true if objects are equal, false otherwise
 * 
 */
bool RT_Thread:: operator==(const RT_Thread& rt_thread) const
{
    bool result;
    result = 
    (
        (this->attr_.sched_policy == rt_thread.attr_.sched_policy) &&
        (this->attr_.sched_period == rt_thread.attr_.sched_period) &&
        (this->attr_.sched_deadline == rt_thread.attr_.sched_deadline)
    );
    return result;
    
}

void Thread_Manager::RunAllThreads(void)
{
    for (auto & thread : collected_threads_) 
    {
        thread.Run();
    }
}