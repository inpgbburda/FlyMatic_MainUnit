#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "mpu6050.hpp"

#include <vector>


TEST_GROUP(balancer)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(balancer, FirstTest)
{
    CHECK_EQUAL(1, 1);
}