#include <iostream>
#include "pwm.hpp"
#include <mutex>
#include <thread>
#include <unistd.h>

extern uint32_t Time_Calibration_G;

void Do_Pwm(void)
{
	while(1){
		Run_Pwm_Blocking();
	}
}

void Do_Main_Routine(void)
{
	std::cout << "Step 1" << std::endl;
	Set_Pwm(CHAN_1, 100U);
	sleep(5);

	std::cout << "Step 2" << std::endl;
	Set_Pwm(CHAN_1, 0U);
	sleep(5);

	std::cout << "Step 3" << std::endl;
	Set_Pwm(CHAN_1, 70U);
	sleep(5);

	std::cout << "Step 4" << std::endl;
	Set_Pwm(CHAN_1, 100U);
	sleep(5);	
}

int main()
{
	std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

	Init_Pwm();
	std::thread thread_pwm(Do_Pwm);
	std::thread thread_main_routine(Do_Main_Routine);

	thread_main_routine.join();
	thread_pwm.join();
	
	return 0;
}
