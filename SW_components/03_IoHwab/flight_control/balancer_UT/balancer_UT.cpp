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

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
        .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(1.0, 0.0, 0);
    balancer->ProcessControl();

    CHECK_EQUAL(10, balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(0, balancer->GetCurrentThrust(MOTOR_2));
}

TEST(Balancer, CalculatesControlSignalForOverRoll)
{
    int32_t roll_angle = 20;
    int32_t target_angle = 10;

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
        .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(1.0, 0.0, 0);
    balancer->ProcessControl();

    CHECK_EQUAL(0,  balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(10, balancer->GetCurrentThrust(MOTOR_2));
}

TEST(Balancer, CalculatesControlSignalForOverRollAndSetBaseThrust)
{
    int32_t roll_angle = 20;
    int32_t target_angle = 10;

    mock().expectOneCall("GetSpiritAngle")
        .ignoreOtherParameters()
        .andReturnValue(roll_angle);
    mock().expectOneCall("ReadWriteData")
          .ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(1.0, 0.0, 0);
    balancer->SetBaseThrust(Base_Thrust);
    balancer->ProcessControl();

    CHECK_EQUAL(15, balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(35, balancer->GetCurrentThrust(MOTOR_2));
}

TEST(Balancer, SetsBaseThrust)
{
    CHECK_EQUAL(Inital_Base_Thrust, balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(Inital_Base_Thrust, balancer->GetCurrentThrust(MOTOR_2));

    balancer -> SetBaseThrust(Base_Thrust);

    CHECK_EQUAL(Base_Thrust, balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(Base_Thrust, balancer->GetCurrentThrust(MOTOR_2));
}

/** Kp=0, Ki=1, Kd=0 
*/
TEST(Balancer, CalculatesControlSignalWithIntegralForOverRoll)
{
    int32_t target_angle = 0;
    
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(10);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(25);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(0, 1, 0);

    balancer->ProcessControl();
    balancer->ProcessControl();

    CHECK_EQUAL(0,  balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(10+25, balancer->GetCurrentThrust(MOTOR_2));
}

/** Kp=0, Ki=0.5, Kd=0 
*/
TEST(Balancer, CalculatesControlSignalWithIntegralValSmallForOverRoll)
{
    int32_t target_angle = 0;
    
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(1);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(2);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(0, 0.5, 0);

    balancer->ProcessControl();
    balancer->ProcessControl();

    CHECK_EQUAL(0, balancer->GetCurrentThrust(MOTOR_1));
    CHECK_EQUAL(2, balancer->GetCurrentThrust(MOTOR_2)); /* 1.5 -round-> 2*/
}

TEST(Balancer, CalculatesControlSignalWithDifferentialForOverRoll)
{
    int32_t target_angle = 0;
    
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(5);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();
    mock().expectOneCall("GetSpiritAngle").ignoreOtherParameters().andReturnValue(10);
    mock().expectOneCall("ReadWriteData").ignoreOtherParameters();

    balancer->SetTargetAngle(target_angle);
    balancer->SetRegulatorConstants(0, 0, 0.1);

    balancer->ProcessControl();
    balancer->ProcessControl();

    // TODO: Choose the right expected values to test kd properly
    // CHECK_EQUAL(0, balancer->GetCurrentThrust(MOTOR_1));
    // CHECK_EQUAL(2, balancer->GetCurrentThrust(MOTOR_2)); 
}
// TODO: Test saturation SPI max value of 255