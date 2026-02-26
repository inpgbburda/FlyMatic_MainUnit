#include "Monotonic_Sleep.hpp"
#include <time.h>

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#define NSEC_PER_MSEC 1000000ULL
#define REMAINING_TIME_UNUSED static_cast<void*>(nullptr)

TEST_GROUP(MonotonicSleep)
{
    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(MonotonicSleep, SleepsMsUninterrupted)
{
    int wait_time_ms = 1;

    timespec now_ts {}; /* arbitrary time which is assumed to be at calling the waiting function */
    now_ts.tv_sec = 0;
    now_ts.tv_nsec = 123;

    timespec wakeup_ts {}; /* time which is expected to wake up */
    wakeup_ts.tv_sec = 0;
    wakeup_ts.tv_nsec = now_ts.tv_nsec + (wait_time_ms * NSEC_PER_MSEC);

    mock().expectOneCall("clock_nanosleep")
          .withParameter("clock_id", static_cast<int>(CLOCK_MONOTONIC))
          .withParameter("flags", TIMER_ABSTIME)
          .withMemoryBufferParameter("request",
                                     reinterpret_cast<const unsigned char*>(&wakeup_ts),
                                     sizeof(timespec))
          .withParameter("remain", REMAINING_TIME_UNUSED)
          .andReturnValue(0);
    
    mock().expectOneCall("clock_gettime")
            .withParameter("clock_id", static_cast<int>(CLOCK_MONOTONIC))
            .andReturnValue(0)
            .withOutputParameterReturning("ts", &now_ts, sizeof(timespec));

    SleepMonotonicRawMs(wait_time_ms);

}