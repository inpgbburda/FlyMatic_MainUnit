#include "Monotonic_Sleep.hpp"

#include <errno.h>
#include <time.h>

#define REMAINING_TIME_UNUSED nullptr

static constexpr uint64_t NSEC_PER_SEC = 1000000000ULL;
static constexpr uint64_t NSEC_PER_USEC = 1000ULL;
static constexpr uint64_t NSEC_PER_MSEC = 1000000ULL;

static constexpr clockid_t SLEEP_CLOCK = CLOCK_MONOTONIC;

static uint64_t GetMonotonicClockNowNs(void);
static int SleepUntilNsWithMonotonicClock(uint64_t wakeup_time_ns);

/**
 * SleepMonotonicRawUs
 * @brief: Sleeps for relative duration in microseconds using monotonic clock
 * @param:time_us - relative sleep duration in microseconds
 *
 * @return: 0 on success, POSIX error code on failure
 * 
 */
int SleepMonotonicRawUs(uint64_t time_us)
{
    uint64_t wakeup_time_ns = GetMonotonicClockNowNs() + (time_us * NSEC_PER_USEC);
    return SleepUntilNsWithMonotonicClock(wakeup_time_ns);
}

/**
 * SleepMonotonicRawMs
 * @brief: Sleeps for relative duration in milliseconds using monotonic clock
 * @param:time_ms - relative sleep duration in milliseconds
 *
 * @return: 0 on success, POSIX error code on failure
 * 
 */
int SleepMonotonicRawMs(uint64_t time_ms)
{
    uint64_t wakeup_time_ns = GetMonotonicClockNowNs() + (time_ms * NSEC_PER_MSEC);
    return SleepUntilNsWithMonotonicClock(wakeup_time_ns);
}

/**
 * GetMonotonicClockNowNs
 * @brief: Reads current timestamp from the monotonic clock source
 *
 * @return: current time in nanoseconds, 0 on read error
 * 
 */
static uint64_t GetMonotonicClockNowNs(void)
{
    timespec ts{};

    int result = clock_gettime(SLEEP_CLOCK, &ts);

    if (result != 0) { /* If clock_gettime fails, return current time as 0 */
        return 0ULL;
    }
    return static_cast<uint64_t>(ts.tv_sec) * NSEC_PER_SEC + static_cast<uint64_t>(ts.tv_nsec);
}

/**
 * SleepUntilNsWithMonotonicClock
 * @brief: Sets thread to sleep until absolute wake-up time using monotonic clock
 * 
 * Some signals can interrupt the sleep, causing clock_nanosleep to return early with an EINTR error.
 * By looping on EINTR, we ensure that the thread continues to sleep until the intended wake-up time is reached,
 * even if it gets interrupted by signals. This makes the sleep function more robust.
 * 
 * @param:wakeup_time_ns - absolute wake-up timestamp in nanoseconds
 *
 * @return: 0 on success, POSIX error code on failure
 * 
 */
static int SleepUntilNsWithMonotonicClock(uint64_t wakeup_time_ns)
{
    int result = 0;
    timespec wakeup_ts {};
    
    wakeup_ts.tv_sec = static_cast<time_t>(wakeup_time_ns / NSEC_PER_SEC);
    wakeup_ts.tv_nsec = static_cast<long>(wakeup_time_ns % NSEC_PER_SEC);

    /* Loop to handle interrupted sleep by signals (EINTR error) */
    do {
        result = clock_nanosleep(SLEEP_CLOCK, TIMER_ABSTIME, &wakeup_ts, REMAINING_TIME_UNUSED);
    }
    while (EINTR == result);

    return result;
}