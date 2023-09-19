#pragma once

typedef enum
{
    CHAN_BEGIN,
    CHAN_1 = CHAN_BEGIN,
    CHAN_2,
    CHAN_3,
    CHAN_4,
    CHAN_END
} 
Gpio_Channel_T;

void Set_Pwm(Gpio_Channel_T channel, int32_t pwm_percentage);