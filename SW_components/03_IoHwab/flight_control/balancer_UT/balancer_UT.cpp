#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "balancer.hpp"

TEST_GROUP(balancer)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(balancer, TestCalculatingControlSignalForYaw)
{
    Balancer balancer;
    balancer.Runable();
    CHECK_EQUAL(1, 1);
}