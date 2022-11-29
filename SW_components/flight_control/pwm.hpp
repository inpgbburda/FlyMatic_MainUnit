#ifndef PWM_HPP
#define PWM_HPP

#include <stdint.h>

#define BUSY_TICKS_TO_US 240U /* Experimentaily adjusted value */

#define MAX_TICS 200U
#define MIN_TICS 100U

#define PWM_PERIOD 2000U    /* Period set to 20ms */

typedef void(*Gpio_Fun_Ptr_T)(int pin);

typedef struct
{
    Gpio_Fun_Ptr_T set_high;
    Gpio_Fun_Ptr_T set_low;
}
GPIO_Interface_T; 

typedef enum
{
    CHAN_1 = 1,
    CHAN_2,
    CHAN_3,
    CHAN_4
} 
Gpio_Channel_T;

extern GPIO_Interface_T Gpio_Interface;


void Run_PWM_Blocking(void);
void Set_PWM(Gpio_Channel_T channel, int32_t pwm_percentage);
int32_t Get_PWM(Gpio_Channel_T channel);
uint32_t Thrust_To_Tics(int32_t percentage);

#endif // PMM_HPP