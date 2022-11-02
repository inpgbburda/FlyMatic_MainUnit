#include"pwm.hpp"

GPIO_Interface_T Gpio_Interface;

uint32_t Thrust_To_Tics(int percentage){
    uint32_t tics = 0;
    tics = ((percentage * (MAX_TICS-MIN_TICS)) / 100) + MIN_TICS;
    return tics;
}

void Run_PWM(void){
    static uint32_t counter_us = 0;
    uint32_t width1 = 2000;
    uint32_t width2 = 1500;
    uint32_t width3 = 1000;
    uint32_t width4 = 1200;

    if(PWM_PERIOD > counter_us)
    {
        if(0 == counter_us)
        {
            (Gpio_Interface.set_high)(1);
            (Gpio_Interface.set_high)(2);
            (Gpio_Interface.set_high)(3);
            (Gpio_Interface.set_high)(4);
        }
        else
        {
            if(width1 == counter_us)
            {
                (Gpio_Interface.set_low)(1);
            }

            if(width2 == counter_us)
            {
                (Gpio_Interface.set_low)(2);
            }

            if(width2 == counter_us)
            {
                (Gpio_Interface.set_low)(3);
            }

            if(width2 == counter_us)
            {
                (Gpio_Interface.set_low)(4);
            }
        }
        counter_us ++;
    }
    else
    {
        counter_us = 0;
    }
    //TODO: Wait 1 us 
}