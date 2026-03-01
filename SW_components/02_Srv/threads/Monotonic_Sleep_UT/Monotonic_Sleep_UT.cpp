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
    
    mock().expectOneCall("clock_gettime")
            .andReturnValue(0)
            .withOutputParameterReturning("ts", &now_ts, sizeof(timespec))
            .ignoreOtherParameters();

    mock().expectOneCall("clock_nanosleep")
          .withMemoryBufferParameter("request",
                                     reinterpret_cast<const unsigned char*>(&wakeup_ts),
                                     sizeof(timespec))
          .andReturnValue(0)
          .ignoreOtherParameters();

    SleepMonotonicRawMs(wait_time_ms);
}

TEST(MonotonicSleep, SleepsMsInterrupted)
{
    int wait_time_ms = 1;

    timespec now_ts {}; /* arbitrary time which is assumed to be at calling the waiting function */
    now_ts.tv_sec = 0;
    now_ts.tv_nsec = 123;

    timespec wakeup_ts {}; /* time which is expected to wake up */
    wakeup_ts.tv_sec = 0;
    wakeup_ts.tv_nsec = now_ts.tv_nsec + (wait_time_ms * NSEC_PER_MSEC);
    
    mock().expectOneCall("clock_gettime")
            .andReturnValue(0)
            .withOutputParameterReturning("ts", &now_ts, sizeof(timespec))
            .ignoreOtherParameters();

    /* First sleep - early wakeup, interrupted by signal */
    mock().expectOneCall("clock_nanosleep")
          .withMemoryBufferParameter("request",
                                     reinterpret_cast<const unsigned char*>(&wakeup_ts),
                                     sizeof(timespec))
          .andReturnValue(EINTR) 
          .ignoreOtherParameters();
    
    /* Second sleep - successful till the end, no interruption */
    mock().expectOneCall("clock_nanosleep")
          .withMemoryBufferParameter("request",
                                     reinterpret_cast<const unsigned char*>(&wakeup_ts),
                                     sizeof(timespec))
          .andReturnValue(0)
          .ignoreOtherParameters();

    CHECK_EQUAL(0, SleepMonotonicRawMs(wait_time_ms));
}