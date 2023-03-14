#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "Thread_Manager.hpp"
#include <iostream>
#define MOCK_SCHED_DEADLINE 6U

static sched_attr_t mock_thread_attr;

static void *MockFunctionThread(void *data_ptr)
{
    sched_attr_t* attr = (sched_attr_t*)data_ptr;
    mock_thread_attr.size = attr->size;
    mock_thread_attr.sched_policy = attr->sched_policy;
    mock_thread_attr.sched_runtime = attr->sched_runtime;
    mock_thread_attr.sched_period = attr->sched_period;
    mock_thread_attr.sched_deadline = attr->sched_deadline;
}

TEST_CASE( "Test passing scheduler attributes to thread", "[RT_Thread]" ) 
{
    RT_Thread thread = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    // thread.Init();
    thread.Run();
    thread.Join();
    REQUIRE(mock_thread_attr.sched_policy == MOCK_SCHED_DEADLINE);
    REQUIRE(mock_thread_attr.sched_runtime == 100);
    REQUIRE(mock_thread_attr.sched_deadline == 200);
    REQUIRE(mock_thread_attr.sched_period == 300);
}

TEST_CASE( "Test aggregating threads", "[Thread_Manager]" ) 
{
    RT_Thread thread_1 = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    RT_Thread thread_2 = RT_Thread(MOCK_SCHED_DEADLINE, 1000, 2000, 3000, &MockFunctionThread);
    RT_Thread thread_3 = RT_Thread(MOCK_SCHED_DEADLINE, 10000, 20000, 30000, &MockFunctionThread);

    std::vector<RT_Thread> initial_thread_list = {thread_1, thread_2, thread_3};

    Thread_Manager manager = Thread_Manager();
    manager.CollectThreads(initial_thread_list);
    
    std::vector<RT_Thread> received_thread_list = manager.GetAllThreads();

    REQUIRE( initial_thread_list == received_thread_list);
}

TEST_CASE( "Test running aggregated threads", "[Thread_Manager]" ) 
{
    RT_Thread thread_1 = RT_Thread(MOCK_SCHED_DEADLINE, 100, 200, 300, &MockFunctionThread);
    RT_Thread thread_2 = RT_Thread(MOCK_SCHED_DEADLINE, 1000, 2000, 3000, &MockFunctionThread);
    RT_Thread thread_3 = RT_Thread(MOCK_SCHED_DEADLINE, 10000, 20000, 30000, &MockFunctionThread);

    std::vector<RT_Thread> initial_thread_list = {thread_1, thread_2, thread_3};
    Thread_Manager manager = Thread_Manager();

    manager.CollectThreads(initial_thread_list);
    manager.RunAllThreads();
    std::vector<RT_Thread> received_thread_list = manager.GetAllThreads();

    CHECK(received_thread_list[0].IsRun());
    CHECK(received_thread_list[1].IsRun());
    CHECK(received_thread_list[2].IsRun());
}
