#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "balancer.hpp"
#include "pwm.hpp"

TEST_GROUP(Balancer)
{
    Balancer* balancer;

    void setup()
    {
        balancer = new Balancer();
    }
    void teardown()
    {
        delete balancer;
        mock().checkExpectations();
        mock().clear();
    }
};

TEST(Balancer, CalculatesControlSignalForYaw)
{
    mock().expectOneCall("Set_Pwm")
        .withParameter("channel", CHAN_1)
        .withParameter("pwm_percentage", 25);

    mock().expectOneCall("Set_Pwm")
        .withParameter("channel", CHAN_2)
        .withParameter("pwm_percentage", 25);

    balancer->ProcessControl();
}