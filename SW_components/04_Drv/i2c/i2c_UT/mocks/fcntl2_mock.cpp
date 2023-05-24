#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
    #include "fcntl2.h"
}

int open(const char *__path, int __oflag, ...)
{
    mock().actualCall("open").withParameter("__path", __path).withParameter("__oflag", __oflag);
    return mock().intReturnValue();
}