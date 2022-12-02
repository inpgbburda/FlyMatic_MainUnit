#include"pwm.hpp"
#ifdef _RASP
#include <wiringPi.h>
#endif
#include "time.h"

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
uint32_t Time_Calibration_G;


static void Set_Pin(int pin);
static void Clear_Pin(int pin);
uint32_t Busy_Wait_Calibrate(void);


uint32_t Busy_Wait_Calibrate(void) {
    struct timespec btime, etime;
    volatile int loop_cnt;
    clock_gettime(CLOCK_REALTIME, &btime);
    for (loop_cnt = 0; loop_cnt < 1000000000; loop_cnt++) 
    {
        /*Do Nothing*/
    };
    clock_gettime(CLOCK_REALTIME, &etime);
    double diff_seconds = (double) (etime.tv_sec - btime.tv_sec);
    double diff_nseconds = (double) (etime.tv_nsec - btime.tv_nsec);
    double nseconds = (double) (diff_seconds* 1000000000L)+ diff_nseconds;

    int loops_per_us = (int) (1000000000 / nseconds * 1000) + TIME_OFFSET;
    return loops_per_us;
} 


void Init_PWM(void){
    Time_Calibration_G = 0;
    #ifdef _RASP
	wiringPiSetup();
	pinMode (PIN_MOTOR_1, OUTPUT);
	pinMode (PIN_MOTOR_2, OUTPUT);
	pinMode (PIN_MOTOR_3, OUTPUT);
	pinMode (PIN_MOTOR_4, OUTPUT);
    pinMode (PIN_DEBUG, OUTPUT);
	#endif
    Gpio_Interface.set_high = &Set_Pin;
    Gpio_Interface.set_low = &Clear_Pin;
    Time_Calibration_G = Busy_Wait_Calibrate();
}


uint32_t Thrust_To_Tics(int32_t percentage){
    uint32_t tics = 0;
    tics = uint32_t( ((percentage * (MAX_TICS-MIN_TICS)) / 100U) + MIN_TICS );
    return tics;
}


/*
   Function must be called with minimum 10us.
   Below this value, generated time will not be accurate.
*/
static void Delay_us(uint32_t micro_seconds){
   volatile uint32_t us_cnt = 0;
   volatile uint32_t cnt = 0;
   for(us_cnt=0; us_cnt<micro_seconds; us_cnt++){
      for(cnt=0; cnt<Time_Calibration_G; cnt++);   
   }
}


void Run_PWM_Blocking(void){

    static uint32_t timer = 0U; /*Every incremented value means 10us passed*/

    if(PWM_PERIOD > timer)
    {
        if(0U == timer)
        {
            Width1 = Thrust_To_Tics(Perc1);
            Width2 = Thrust_To_Tics(Perc2);
            Width3 = Thrust_To_Tics(Perc3);
            Width4 = Thrust_To_Tics(Perc4);

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


static void Set_Pin(int pin){
    #ifdef _RASP
    if(1 == pin){
        digitalWrite (PIN_MOTOR_1, HIGH);
    }
    if(2 == pin){
        digitalWrite (PIN_MOTOR_2, HIGH);
    }
    if(3 == pin){
        digitalWrite (PIN_MOTOR_3, HIGH);
    }
    if(4 == pin){
        digitalWrite (PIN_MOTOR_4, HIGH);
    }
    #endif
}

static void Clear_Pin(int pin){
    #ifdef _RASP
    if(1 == pin){
        digitalWrite (PIN_MOTOR_1, LOW);
    }
    if(2 == pin){
        digitalWrite (PIN_MOTOR_2, LOW);
    }
    if(3 == pin){
        digitalWrite (PIN_MOTOR_3, LOW);
    }
    if(4 == pin){
        digitalWrite (PIN_MOTOR_4, LOW);
    }
    #endif
}