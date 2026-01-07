#include "Thread_Manager.hpp"
#include <vector>

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#define MOCK_SCHED_DEADLINE 6U

static sched_attr_t mock_thread_attr;

static void *MockFunctionThread(void *data_ptr)
{
    RT_Thread_StartPayload* payload = (RT_Thread_StartPayload*)data_ptr;
    sched_attr_t* attr = payload->attr_ptr;
    mock_thread_attr.size = attr->size;
    mock_thread_attr.sched_policy = attr->sched_policy;
    mock_thread_attr.sched_runtime = attr->sched_runtime;
    mock_thread_attr.sched_period = attr->sched_period;
    mock_thread_attr.sched_deadline = attr->sched_deadline;
    return nullptr;
}

TEST_GROUP(RT_ThreadGroup)
{
};

TEST(RT_ThreadGroup, PassSchedulerAttributesToThread)
{
    RT_Thread thread = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    thread.Run();
    thread.Join();
    CHECK_TRUE(mock_thread_attr.sched_policy == MOCK_SCHED_DEADLINE);
    CHECK_TRUE(mock_thread_attr.sched_runtime == 100);
    CHECK_TRUE(mock_thread_attr.sched_deadline == 200);
    CHECK_TRUE(mock_thread_attr.sched_period == 300);
}

TEST_GROUP(ThreadManagerGroup)
{
};

TEST(ThreadManagerGroup, AggregatesThreads)
{
    RT_Thread thread_1 = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    RT_Thread thread_2 = RT_Thread(MOCK_SCHED_DEADLINE, 1000, 2000, 3000, &MockFunctionThread);
    RT_Thread thread_3 = RT_Thread(MOCK_SCHED_DEADLINE, 10000, 20000, 30000, &MockFunctionThread);

    std::vector<RT_Thread> initial_thread_list = {thread_1, thread_2, thread_3};

    Thread_Manager manager = Thread_Manager();
    manager.Init(initial_thread_list);

    std::vector<RT_Thread> received_thread_list = manager.GetAllThreads();

    CHECK_TRUE(initial_thread_list == received_thread_list);
}

TEST(ThreadManagerGroup, RunsAggregatedThreads)
{
    RT_Thread thread_1 = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    RT_Thread thread_2 = RT_Thread(MOCK_SCHED_DEADLINE, 1000, 2000, 3000, &MockFunctionThread);
    RT_Thread thread_3 = RT_Thread(MOCK_SCHED_DEADLINE, 10000, 20000, 30000, &MockFunctionThread);

    std::vector<RT_Thread> initial_thread_list = {thread_1, thread_2, thread_3};
    Thread_Manager manager = Thread_Manager();

    manager.Init(initial_thread_list);
    manager.RunAllThreads();
    std::vector<RT_Thread> received_thread_list = manager.GetAllThreads();

    CHECK_TRUE(received_thread_list[0].IsRun());
    CHECK_TRUE(received_thread_list[1].IsRun());
    CHECK_TRUE(received_thread_list[2].IsRun());
}
