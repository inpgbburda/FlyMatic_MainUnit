#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "pwm.hpp"

void Set_Pwm(Gpio_Channel_T channel, int32_t pwm_percentage)
{
    mock().actualCall("Set_Pwm")
        .withParameter("channel", channel)
        .withParameter("pwm_percentage", pwm_percentage);
}