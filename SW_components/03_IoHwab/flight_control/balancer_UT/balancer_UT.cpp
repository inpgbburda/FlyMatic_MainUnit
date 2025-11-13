#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "balancer.hpp"

/*
*    Testing constant configuration
*/

const int32_t Inital_Base_Thrust = 0;
const int32_t Base_Thrust = 25;
static const int SPI_CHANNEL = 1;

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
/** Kp=1, Ki=0, Kd=0 
r - target angle
y - current angle
e = r - y
e - error
u - control signal
u = Kp*e + Ki*∫e dt + Kd*de/dt
thrust_motor_1 = base_thrust - u
thrust_motor_2 = base_thrust + u
*/

TEST(Balancer, CalculatesControlSignalForUnderRoll)
{
    int32_t roll_angle = 0;
    int32_t target_angle = 10;
    uint8_t exp_spi_buffer[] = {10, 0, 0, 0};

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
        .withParameter("channel", SPI_CHANNEL)
        .withMemoryBufferParameter("buffer", exp_spi_buffer, sizeof(exp_spi_buffer))
        .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->ProcessControl();
}

TEST(Balancer, CalculatesControlSignalForOverRoll)
{
    int32_t roll_angle = 20;
    int32_t target_angle = 10;
    uint8_t exp_spi_buffer[] = {0, 10, 0, 0};

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
        .withParameter("channel", SPI_CHANNEL)
        .withMemoryBufferParameter("buffer", exp_spi_buffer, sizeof(exp_spi_buffer))
        .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->ProcessControl();
}

TEST(Balancer, CalculatesControlSignalForOverRollAndSetBaseThrust)
{
    int32_t roll_angle = 20;
    int32_t target_angle = 10;
    uint8_t exp_spi_buffer[] = {15, 35, 0, 0};

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
        .withParameter("channel", SPI_CHANNEL)
        .withMemoryBufferParameter("buffer", exp_spi_buffer, sizeof(exp_spi_buffer))
        .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer -> SetBaseThrust(Base_Thrust);
    balancer->ProcessControl();
}

/** Kp=0, Ki=1, Kd=0 
*/

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