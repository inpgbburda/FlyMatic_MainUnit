#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

int ioctl (int __fd, unsigned long int __request, int addr)
{
    mock().actualCall("ioctl")
        .withParameter("__fd", __fd)
        .withParameter("__request", __request)
        .withParameter("addr", addr);
        
    return mock().intReturnValue();
}