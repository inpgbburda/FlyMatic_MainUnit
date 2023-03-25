#include "Thread_Manager.hpp"
#include <string.h>             /* Needed for memset() call*/

#include "Thread_Manager_Cfg.hpp"

#define DEFAULT_PID 0U
#define DEFAULT_FLAGS 0U

void SchedSetAttr(sched_attr_t *attr_ptr) 
{
    int result;
    sched_attr_t attr_local;
    memset(&attr_local, 0, sizeof(attr_local)); 
    //TODO: change to shorter copying
    attr_local.size = attr_ptr->size;
    attr_local.sched_policy = attr_ptr->sched_policy ;
    attr_local.sched_runtime = attr_ptr->sched_runtime;
    attr_local.sched_deadline = attr_ptr->sched_deadline;
    attr_local.sched_period = attr_ptr->sched_period;
    result = syscall(__NR_sched_setattr, DEFAULT_PID, &attr_local, DEFAULT_FLAGS);
    if(result < 0)
    {
        // throw std::runtime_error("sched_setattr failed to set the priorities");
        std::cout << "sched_setattr failed to set the priorities"<< std::endl;
    }
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
}

void RT_Thread::Run(void)
{
    exec_state_ = true;
    pthread_create(&posix_instance_, NULL, fun_ptr_, (void*)&attr_);
}

void RT_Thread::AssignAffinity(void)
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