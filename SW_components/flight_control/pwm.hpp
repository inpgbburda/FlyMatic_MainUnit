#ifndef PWM_HPP
#define PWM_HPP

#include <stdint.h>
#include <stdexcept>
#include "pwm_cfg.hpp"

#define TIME_OFFSET  6
#define CALIB_TICKS  1000000000U
#define NS_IN_SECOND 1000000000L
#define NS_IN_MICROSECOND 1000


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


typedef void(*Gpio_Fun_Ptr_T)(int pin, bool state);

typedef struct
{
    Gpio_Fun_Ptr_T set_pin_state_fptr;
}
GPIO_Interface_T; 


extern GPIO_Interface_T Gpio_Interface;
extern uint32_t Time_Calibration_G;


void Init_Pwm(void);
void Run_Pwm_Blocking(void);
void Set_Pwm(Gpio_Channel_T channel, int32_t pwm_percentage);
int32_t Get_Pwm(Gpio_Channel_T channel);

#endif // PMM_HPP