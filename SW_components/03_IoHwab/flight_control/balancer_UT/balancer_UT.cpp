#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "balancer.hpp"

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
/* P=1, I=0, D=0 */
TEST(Balancer, CalculatesControlSignalForRoll)
{
    uint8_t exp_spi_buffer[] = {0x00, 0x00, 0x00, 0x00};

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(0);
    mock().expectOneCall("ReadWriteData")
        .withParameter("channel", 1)
        .withMemoryBufferParameter("buffer", exp_spi_buffer, sizeof(exp_spi_buffer))
        .ignoreOtherParameters();

    balancer->SetTargetAngle(10);
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