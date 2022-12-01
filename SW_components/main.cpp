#include <iostream>
#include "pwm.hpp"

using namespace std;

int main()
{
	cout << "Witam serdecznie w projekcie drona"<< endl;

	Init_PWM();
	Set_PWM(CHAN_1, 0U);
	Set_PWM(CHAN_2, 75U);
	Set_PWM(CHAN_3, 50U);
	Set_PWM(CHAN_4, 25U);
		
	while(1){
		Run_PWM_Blocking();
	}
	return 0;
}
