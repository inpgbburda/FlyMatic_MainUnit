#pragma once

#include <pthread.h>
#include <cstdint>

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
    sched_attr_t* attr_ptr; /*Thread scheduling attributes*/
    void* user_arg;         /*User argument passed to thread function*/
}
RT_Thread_StartPayload;

void SchedSetAttr(sched_attr_t *attr_ptr);
void PreventPagingToSwapArea(void);
void sleep(int time);