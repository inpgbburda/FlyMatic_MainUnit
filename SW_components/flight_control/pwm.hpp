#ifndef PWM_HPP
#define PWM_HPP

#include <stdint.h>

#define MAX_TICS 2000
#define MIN_TICS 1000

#define PWM_PERIOD 20000U

typedef void(*Gpio_Fun_Ptr_T)(int pin);

typedef struct
{
    Gpio_Fun_Ptr_T set_high;
    Gpio_Fun_Ptr_T set_low;
}GPIO_Interface_T; 

extern GPIO_Interface_T Gpio_Interface;


uint32_t Thrust_To_Tics(int percentage);
void Run_PWM(void);

#endif // PMM_HPP