#include"pwm.hpp"
#ifdef _RASP
#include <wiringPi.h>
#endif
#include "time.h"
#include "Thread_Manager.hpp"

uint32_t Time_Calibration_G;


static uint32_t Width_Array[CHAN_END];
static uint32_t Perc_Array[CHAN_END];
static GPIO_Interface_T Gpio_Interface;


Gpio_Channel_T& operator ++ (Gpio_Channel_T& chan);


/*
|===================================================================================================================================|
    Exported function prototypes
|===================================================================================================================================|
*/
void Init_Pwm(void);
void Set_Pwm(Gpio_Channel_T channel, int32_t pwm_percentage);
int32_t Get_Pwm(Gpio_Channel_T channel);
void Run_Pwm_Blocking(void);


/*
|===================================================================================================================================|
    Local function prototypes
|===================================================================================================================================|
*/
static uint32_t Convert_Thrust_To_Tics(int32_t percentage);
static void Set_Pin_State(int wpi_pin, bool state);
static uint32_t Busy_Wait_Calibrate(void);
static void Delay_us(uint32_t micro_seconds);


/*
|===================================================================================================================================|
    Function definitions
|===================================================================================================================================|
*/
void Init_Pwm(void){
    Time_Calibration_G = 0;
    #ifdef _RASP
	wiringPiSetup();
	pinMode (PIN_MOTOR_1, OUTPUT);
	pinMode (PIN_MOTOR_2, OUTPUT);
	pinMode (PIN_MOTOR_3, OUTPUT);
	pinMode (PIN_MOTOR_4, OUTPUT);
    pinMode (PIN_DEBUG, OUTPUT);
	#endif
    Gpio_Interface.set_pin_state_fptr = &Set_Pin_State;
    Time_Calibration_G = Busy_Wait_Calibrate();
}


void Set_Pwm(Gpio_Channel_T channel, int32_t pwm_percentage){
     
    for (Gpio_Channel_T chann_cnt = Gpio_Channel_T::CHAN_BEGIN; chann_cnt!=Gpio_Channel_T::CHAN_END; ++chann_cnt) 
    {
        if (channel == chann_cnt)
        {
            Perc_Array[chann_cnt] = pwm_percentage;
            break;
        }
    }
}


int32_t Get_Pwm(Gpio_Channel_T channel){
    
    int32_t read_percentage = 0;

    for (Gpio_Channel_T chann_cnt = Gpio_Channel_T::CHAN_BEGIN; chann_cnt!=Gpio_Channel_T::CHAN_END; ++chann_cnt) 
    {
        if (channel == chann_cnt)
        {
            read_percentage = Perc_Array[chann_cnt];
            break;
        }
    }
    return read_percentage;
}


void Run_Pwm_Blocking(void){

    static uint32_t timer = 0U; /*Every incremented value means 10us passed*/

    if(PWM_PERIOD > timer)
    {
        if(0U == timer)
        {
            /*Start of the period - All channels begin duty cycle*/
            pthread_mutex_trylock(&Pwm_lock_G);
            Width_Array[CHAN_1] = Convert_Thrust_To_Tics(Perc_Array[CHAN_1]);
            Width_Array[CHAN_2] = Convert_Thrust_To_Tics(Perc_Array[CHAN_2]);
            Width_Array[CHAN_3] = Convert_Thrust_To_Tics(Perc_Array[CHAN_3]);
            Width_Array[CHAN_4] = Convert_Thrust_To_Tics(Perc_Array[CHAN_4]);
            pthread_mutex_unlock(&Pwm_lock_G);

            (Gpio_Interface.set_pin_state_fptr)(Pwm_Chann_To_Pin_Map[CHAN_1], true); // TODO: fix passing here channels
            (Gpio_Interface.set_pin_state_fptr)(Pwm_Chann_To_Pin_Map[CHAN_2], true);
            (Gpio_Interface.set_pin_state_fptr)(Pwm_Chann_To_Pin_Map[CHAN_3], true);
            (Gpio_Interface.set_pin_state_fptr)(Pwm_Chann_To_Pin_Map[CHAN_4], true);
        }
        else
        {
            /*Period continues - Check required states for all channels*/
            for (Gpio_Channel_T chann_cnt = Gpio_Channel_T::CHAN_BEGIN; chann_cnt!=Gpio_Channel_T::CHAN_END; ++chann_cnt)
            {
                if(Width_Array[chann_cnt] == timer)
                    {
                        int hw_pin = Pwm_Chann_To_Pin_Map[chann_cnt];
                        (Gpio_Interface.set_pin_state_fptr)(hw_pin, false); //TODO: fix passing here chann_cnt
                    }
            }
        }
        timer ++;
    }
    else
    {
        timer = 0U;
    }
    Delay_us(PWM_TICK_DURATION);
 
}


static uint32_t Convert_Thrust_To_Tics(int32_t percentage){
    uint32_t tics = 0;
    tics = uint32_t( ((percentage * (MAX_TICS-MIN_TICS)) / 100U) + MIN_TICS );
    return tics;
}


static void Set_Pin_State(int wpi_pin, bool state){
    #ifdef _RASP
        digitalWrite (wpi_pin, state);
    #endif
}


static uint32_t Busy_Wait_Calibrate(void) {
    struct timespec btime, etime;
    volatile uint32_t loop_cnt;
    clock_gettime(CLOCK_REALTIME, &btime);
    for (loop_cnt = 0; loop_cnt < CALIB_TICKS; loop_cnt++) 
    {
        /*Do Nothing*/
    };
    clock_gettime(CLOCK_REALTIME, &etime);
    double diff_seconds = (double) (etime.tv_sec - btime.tv_sec);
    double diff_nseconds = (double) (etime.tv_nsec - btime.tv_nsec);
    double nseconds = (double) (diff_seconds* NS_IN_SECOND)+ diff_nseconds;

    int loops_per_us = (int) (CALIB_TICKS / nseconds * NS_IN_MICROSECOND) + TIME_OFFSET;
    return loops_per_us;
}


/*
   Delay_us function must be called with minimum 10us.
   Below this value, generated time will not be accurate.
*/
static void Delay_us(uint32_t micro_seconds){
   volatile uint32_t us_cnt = 0;
   volatile uint32_t cnt = 0;
   for(us_cnt=0; us_cnt<micro_seconds; us_cnt++){
      for(cnt=0; cnt<Time_Calibration_G; cnt++);   
   }
}


Gpio_Channel_T& operator ++ (Gpio_Channel_T& chan)
{
    if (chan == Gpio_Channel_T::CHAN_END) {
        throw std::out_of_range("for Gpio_Channel_T& operator ++ (Gpio_Channel_T&)");
    }
    chan = Gpio_Channel_T(static_cast<std::underlying_type<Gpio_Channel_T>::type>(chan) + 1);
    return chan;
}

void *DoPwm(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    std::cout << "Started pwm routine"<< std::endl;
    while(1){
        Run_Pwm_Blocking();
    }
}