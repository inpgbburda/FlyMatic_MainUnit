#include "Monotonic_Sleep.hpp"

#include <errno.h>
#include <time.h>

#define REMAINING_TIME_UNUSED nullptr
#define MAX_SLEEP_TIME_US 999999U

static constexpr uint64_t NSEC_PER_SEC = 1000000000ULL;
static constexpr uint64_t NSEC_PER_USEC = 1000ULL;
static constexpr uint64_t NSEC_PER_MSEC = 1000000ULL;

static constexpr clockid_t SLEEP_CLOCK = CLOCK_MONOTONIC;

static int GetMonotonicClockNowNs(uint64_t* ns);
static int SleepUntilNsWithMonotonicClock(uint64_t wakeup_time_ns);

/**
 * SleepMonotonicUs
 * @brief: Sleeps for relative duration in microseconds using monotonic clock
 * @param:time_us - relative sleep duration in microseconds
 *
 * @return: 0 on success, POSIX error code on failure
 * 
 */
int SleepMonotonicUs(uint32_t time_us)
{
    uint64_t wakeup_time_ns = 0;

    if(MAX_SLEEP_TIME_US < time_us) { /* If requested sleep time exceeds 100 ms, return overflow error */
        return EINVAL;
    }
    int result = GetMonotonicClockNowNs(&wakeup_time_ns);
    if (0 != result) {
        return result;
    }
    if(UINT64_MAX - wakeup_time_ns < (static_cast<uint64_t>(time_us) * NSEC_PER_USEC)) { /* Check for potential overflow */
        return EOVERFLOW;
    }
    wakeup_time_ns += (static_cast<uint64_t>(time_us) * NSEC_PER_USEC);
    return SleepUntilNsWithMonotonicClock(wakeup_time_ns);
}

/**
 * SleepMonotonicMs
 * @brief: Sleeps for relative duration in milliseconds using monotonic clock
 * @param:time_ms - relative sleep duration in milliseconds
 *
 * @return: 0 on success, POSIX error code on failure
 * 
 */
int SleepMonotonicMs(uint32_t time_ms)
{
    uint64_t wakeup_time_ns = 0;

    int result = GetMonotonicClockNowNs(&wakeup_time_ns);
    if (0 != result) {
        return result;
    }

    wakeup_time_ns += (static_cast<uint64_t>(time_ms) * NSEC_PER_MSEC);
    
    return SleepUntilNsWithMonotonicClock(wakeup_time_ns);
}

/**
 * GetMonotonicClockNowNs
 * @brief: Reads current timestamp from the monotonic clock source
 *
 * @return: current time in nanoseconds, 0 on read error
 * 
 */
static int GetMonotonicClockNowNs(uint64_t* ns)
{
    timespec ts{};

    int result = clock_gettime(SLEEP_CLOCK, &ts);
    if (0 != result) { /* If clock_gettime fails, return current time as 0 */
        int saved = errno;
        return saved;
    }
    *ns = static_cast<uint64_t>(ts.tv_sec * NSEC_PER_SEC + ts.tv_nsec);
    return 0;
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