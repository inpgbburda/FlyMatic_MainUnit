#include <iostream>
#include "pwm.hpp"

using namespace std;
extern uint32_t Time_Calibration_G;

int main()
{
	cout << "Witam serdecznie w projekcie drona"<< endl;

	Init_PWM();
	Set_PWM(CHAN_1, 0U);
	Set_PWM(CHAN_2, 75U);
	Set_PWM(CHAN_3, 50U);
	Set_PWM(CHAN_4, 100U);
	cout << Time_Calibration_G << endl;
	while(1){
		Run_PWM_Blocking();
	}
	return 0;
}
