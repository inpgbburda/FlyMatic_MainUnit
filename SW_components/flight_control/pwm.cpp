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
    tics = uint32_t( ((percentage * (MAX_TICS-MIN_TICS)) / 100U) + MIN_TICS );
    return tics;
}


/*
   Function must be called with minimum 10us.
   Below this value, generated time will not be accurate.
*/
static void Delay_us(int micro_seconds){
   volatile int us_cnt = 0;
   volatile int cnt = 0;
   for(us_cnt=0; us_cnt<micro_seconds; us_cnt++){
      for(cnt=0; cnt<BUSY_TICKS_TO_US; cnt++);   
   }
}

void Run_PWM_Blocking(void){

    static uint32_t timer = 0U; /*Every incremented value means 10us passed*/

    Width1 = Thrust_To_Tics(Perc1);
    Width2 = Thrust_To_Tics(Perc2);
    Width3 = Thrust_To_Tics(Perc3);
    Width4 = Thrust_To_Tics(Perc4);

    if(PWM_PERIOD > timer)
    {
        if(0U == timer)
        {
            (Gpio_Interface.set_high)(CHAN_1);
            (Gpio_Interface.set_high)(CHAN_2);
            (Gpio_Interface.set_high)(CHAN_3);
            (Gpio_Interface.set_high)(CHAN_4);
        }
        else
        {
            if(Width1 == timer)
            {
                (Gpio_Interface.set_low)(CHAN_1);
            }

            if(Width2 == timer)
            {
                (Gpio_Interface.set_low)(CHAN_2);
            }

            if(Width3 == timer)
            {
                (Gpio_Interface.set_low)(CHAN_3);
            }

            if(Width4 == timer)
            {
                (Gpio_Interface.set_low)(CHAN_4);
            }
        }
        timer ++;
    }
    else
    {
        timer = 0U;
    }
    Delay_us(10U);
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