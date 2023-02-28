#include "Thread_Manager.hpp"
#include <string.h>             /* Needed for memset() call*/


int SchedSetAttr(pid_t pid, sched_attr_t *attr_ptr,unsigned int flags) 
{
    sched_attr_t attr_local;
    memset(&attr_local, 0, sizeof(attr_local)); 
    attr_local.size = attr_ptr->size;
    attr_local.sched_policy = attr_ptr->sched_policy ;
    attr_local.sched_runtime = attr_ptr->sched_runtime;
    attr_local.sched_deadline = attr_ptr->sched_deadline;
    attr_local.sched_period = attr_ptr->sched_period;
    return syscall(__NR_sched_setattr, pid, &attr_local, flags);
}