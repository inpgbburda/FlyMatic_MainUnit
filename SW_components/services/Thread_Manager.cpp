#include "Thread_Manager.hpp"

int Set_Scheduler_Attributes(const struct sched_attr* data_ptr, int flags){
    int result;
    result = syscall(__NR_sched_setattr, 0U, data_ptr, flags);
    return result;
}