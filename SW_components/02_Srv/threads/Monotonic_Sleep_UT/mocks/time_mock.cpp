#include <CppUTestExt/MockSupport.h>

#include <errno.h>
#include <time.h>

extern "C" int clock_gettime(clockid_t clock_id, struct timespec* ts)
{
    int errno_out = 0;
    int result = mock().actualCall("clock_gettime")
        .withParameter("clock_id", static_cast<int>(clock_id))
        .withOutputParameter("ts", ts)
        .withOutputParameter("errno_out", &errno_out)
        .returnIntValueOrDefault(0);
    /* Simulate errno setting */
    if (-1 == result) {
        errno = errno_out;
    }
    return result;
}

extern "C" int clock_nanosleep(clockid_t clock_id,
                                int flags,
                                const struct timespec* request,
                                struct timespec* remain)
{
    return mock().actualCall("clock_nanosleep")
        .withParameter("clock_id", static_cast<int>(clock_id))
        .withParameter("flags", flags)
        .withMemoryBufferParameter("request",
                                   reinterpret_cast<const unsigned char*>(request),
                                   sizeof(timespec))
        .withParameter("remain", remain)
        .returnIntValueOrDefault(0);
}
