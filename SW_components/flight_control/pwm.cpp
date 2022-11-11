#include"pwm.hpp"

#ifdef _UNIT_TEST
uint32_t Width1 = 0;
uint32_t Width2 = 0;
uint32_t Width3 = 0;
uint32_t Width4 = 0;

uint32_t Perc1 = 0;
uint32_t Perc2 = 0;
uint32_t Perc3 = 0;
uint32_t Perc4 = 0;

#else
static uint32_t Width1 = 0;
static uint32_t Width2 = 0;
static uint32_t Width3 = 0;
static uint32_t Width4 = 0;

static uint32_t Perc1 = 0;
static uint32_t Perc2 = 0;
static uint32_t Perc3 = 0;
static uint32_t Perc4 = 0;
#endif


GPIO_Interface_T Gpio_Interface;

uint32_t Thrust_To_Tics(int32_t percentage){
    uint32_t tics = 0;
    tics = uint32_t( ((percentage * (MAX_TICS-MIN_TICS)) / 100) + MIN_TICS );
    return tics;
}

void Run_PWM_Blocking(void){

    static uint32_t counter_us = 0U;
    if(PWM_PERIOD > counter_us)
    {
        if(0U == counter_us)
        {
            (Gpio_Interface.set_high)(1);
            (Gpio_Interface.set_high)(2);
            (Gpio_Interface.set_high)(3);
            (Gpio_Interface.set_high)(4);
        }
        else
        {
            if(Width1 == counter_us)
            {
                (Gpio_Interface.set_low)(1);
            }

            if(Width2 == counter_us)
            {
                (Gpio_Interface.set_low)(2);
            }

            if(Width3 == counter_us)
            {
                (Gpio_Interface.set_low)(3);
            }

            if(Width4 == counter_us)
            {
                (Gpio_Interface.set_low)(4);
            }
        }
        counter_us ++;
    }
    else
    {
        counter_us = 0U;
    }
    //TODO: Wait 1 us 
}

void Set_PWM(Gpio_Channel_T channel, int32_t pwm_percentage){
    if(CHAN_1 == channel)
    {
        Perc1 = pwm_percentage;
    }
    else if(CHAN_2 == channel)
    {
        Perc2 = pwm_percentage;
    }
    else if(CHAN_3 == channel)
    {
        Perc3 = pwm_percentage;
    }
    else if(CHAN_4 == channel)
    {
        Perc4 = pwm_percentage;
    }
    else{

    }
}

int32_t Get_PWM(Gpio_Channel_T channel){
    int32_t result = 0;
    if(CHAN_1 == channel)
    {
        result = Perc1;
    }
    else if(CHAN_2 == channel)
    {
        result = Perc2;
    }
    else if(CHAN_3 == channel)
    {
        result = Perc3;
    }
    else if(CHAN_4 == channel)
    {
        result = Perc4;
    }
    else{

    }
    return result;
}