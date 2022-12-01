#include <iostream>
#include "pwm.hpp"

#ifdef _RASP
#include <wiringPi.h>
#endif

using namespace std;

int main()
{
	cout << "Witam serdecznie"<< endl;

	wiringPiSetup();
	Set_PWM(CHAN_1, 100);
	
	while(1){
		Run_PWM_Blocking();
	}
	return 0;
}
