#include "Thread_Manager.hpp"
#include <string.h>             /* Needed for memset() call*/

int SetSchedulerAttributes(void* data_ptr, int flags){
    int result;
    memset((void*)data_ptr, 0, sizeof(const struct sched_attr)); 
    result = syscall(__NR_sched_setattr, 0U, data_ptr, flags);
    return result;
}