#include "Thread_Manager.hpp"
#include <string.h>             /* Needed for memset() call*/

#define DEFAULT_PID 0U
#define DEFAULT_FLAGS 0U

int SchedSetAttr(sched_attr_t *attr_ptr) 
{
    sched_attr_t attr_local;
    memset(&attr_local, 0, sizeof(attr_local)); 
    #ifdef _RASP
    attr_local.size = attr_ptr->size;
    attr_local.sched_policy = attr_ptr->sched_policy ;
    attr_local.sched_runtime = attr_ptr->sched_runtime;
    attr_local.sched_deadline = attr_ptr->sched_deadline;
    attr_local.sched_period = attr_ptr->sched_period;
    #endif /* _RASP */
    return syscall(__NR_sched_setattr, DEFAULT_PID, &attr_local, DEFAULT_FLAGS);
}