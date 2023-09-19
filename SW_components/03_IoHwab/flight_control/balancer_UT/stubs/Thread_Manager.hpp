#pragma once

#include <pthread.h>

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

void SchedSetAttr(sched_attr_t *attr_ptr);
void PreventPagingToSwapArea(void);
void sleep(int time);