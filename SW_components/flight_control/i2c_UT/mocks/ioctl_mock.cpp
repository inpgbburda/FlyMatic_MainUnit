#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

int ioctl (int __fd, unsigned long int __request, int addr)
{
    mock().actualCall("ioctl");
    return mock().intReturnValue();
}