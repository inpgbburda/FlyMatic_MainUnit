#ifndef PWM_CFG_HPP
#define PWM_CFG_HPP

#define MAX_TICS 200U
#define MIN_TICS 100U

#define PWM_PERIOD 2000U /* Period set to 20ms */
#define PWM_TICK_DURATION 10U

/*Hardware pins configuration*/
#define PIN_MOTOR_1 7   /*Physical  7*/
#define PIN_MOTOR_2 0   /*Physical 11*/
#define PIN_MOTOR_3 2   /*Physical 13*/
#define PIN_MOTOR_4 3   /*Physical 15*/
#define PIN_DEBUG 1

const int Pwm_Chann_To_Pin_Map[] =
{
   PIN_MOTOR_1,
   PIN_MOTOR_2,
   PIN_MOTOR_3,
   PIN_MOTOR_4
};

#endif // PWM_CFG_HPP