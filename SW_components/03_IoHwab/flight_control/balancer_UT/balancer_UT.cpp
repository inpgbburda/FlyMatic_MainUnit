#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "balancer.hpp"
#include "pwm.hpp"

/*
*    Testing constant configuration
*/

const int32_t Inital_Base_Thrust = 0;
const int32_t Base_Thrust = 25;


/*
*    Test scenarios
*/
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

TEST(Balancer, SetsBaseThrust)
{
    int thrust_1 = balancer->GetCurrentThrust(MOTOR_1);
    int thrust_2 = balancer->GetCurrentThrust(MOTOR_2);

    CHECK_EQUAL(thrust_1, Inital_Base_Thrust);
    CHECK_EQUAL(thrust_2, Inital_Base_Thrust);

    balancer -> SetBaseThrust(Base_Thrust);

    CHECK_EQUAL(balancer->GetCurrentThrust(MOTOR_1), Base_Thrust);
    CHECK_EQUAL(balancer->GetCurrentThrust(MOTOR_2), Base_Thrust);
}