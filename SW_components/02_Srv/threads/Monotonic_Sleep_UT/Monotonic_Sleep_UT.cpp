#include "Monotonic_Sleep.hpp"
#include <time.h>

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#define NSEC_PER_MSEC 1000000ULL
#define NSEC_PER_USEC 1000ULL
#define REMAINING_TIME_UNUSED static_cast<void*>(nullptr)

#define MAX_SLEEP_TIME_US 999999U

TEST_GROUP(MonotonicSleep)
{
    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }
};


TEST(MonotonicSleep, SleepsMsPropperly)
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

    SleepMonotonicMs(wait_time_ms);
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

    CHECK_EQUAL(0, SleepMonotonicMs(wait_time_ms));
}

TEST(MonotonicSleep, SleepFailsToGetCurrentTime)
{
    int arbitrary_wait_time_ms = 1;
    int fake_errno = EFAULT;

    mock().expectOneCall("clock_gettime")
            .andReturnValue(-1) /* Simulate failure to read current time */
            .withOutputParameterReturning("errno_out", &fake_errno, sizeof(fake_errno))
            .ignoreOtherParameters();
    
    int result = SleepMonotonicMs(arbitrary_wait_time_ms);

    CHECK_EQUAL(EFAULT, result);
}

TEST(MonotonicSleep, SleepFailsToNanosleep)
{
    int wait_time_us = 30;

    timespec now_ts {}; /* arbitrary time which is assumed to be at calling the waiting function */
    now_ts.tv_sec = 0;
    now_ts.tv_nsec = 42;

    timespec wakeup_ts {}; /* time which is expected to wake up */
    wakeup_ts.tv_sec = 0;
    wakeup_ts.tv_nsec = now_ts.tv_nsec + (wait_time_us * NSEC_PER_USEC);
    
    mock().expectOneCall("clock_gettime")
            .andReturnValue(0)
            .withOutputParameterReturning("ts", &now_ts, sizeof(timespec))
            .ignoreOtherParameters();

    mock().expectOneCall("clock_nanosleep")
          .withMemoryBufferParameter("request",
                                     reinterpret_cast<const unsigned char*>(&wakeup_ts),
                                     sizeof(timespec))
          .andReturnValue(EFAULT) 
          .ignoreOtherParameters();
    
    int result = SleepMonotonicUs(wait_time_us);

    CHECK_EQUAL(EFAULT, result);
}

TEST(MonotonicSleep, SleepsUsForLongDuration)
{
    uint32_t wait_time_us = MAX_SLEEP_TIME_US + 1;

    int result = SleepMonotonicUs(wait_time_us);

    CHECK_EQUAL(EINVAL, result);
}

/* Test behaviour when sleep is overflowed with large values caused by being called too late */
TEST(MonotonicSleep, SleepUsCalledLately)
{
    /* wait for assummed max time */
    uint32_t wait_time_us = MAX_SLEEP_TIME_US;

    /* simulate very high value for current time */
    timespec now_ts {}; /* arbitrary time which is assumed to be at calling the waiting function */
    now_ts.tv_sec = 0;
    now_ts.tv_nsec = UINT64_MAX - (wait_time_us * NSEC_PER_USEC) + 1; /* Set current time close to max uint64_t to cause overflow */
    
    mock().expectOneCall("clock_gettime")
        .andReturnValue(0)
        .withOutputParameterReturning("ts", &now_ts, sizeof(timespec))
        .ignoreOtherParameters();

    int result = SleepMonotonicUs(wait_time_us);

    CHECK_EQUAL(EOVERFLOW, result);
}

//Test behaviour when calling is done so late, that even GetMonotonicClockNowNs gets overflowed an

TEST(MonotonicSleep, SleepsForZeroUs)
{
    //Maybe return your own ERR code?
}