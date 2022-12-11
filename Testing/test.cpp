#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "pwm.cpp"
#include "pwm.hpp"


static bool pin1_state = false;
static bool pin2_state = false;
static bool pin3_state = false;
static bool pin4_state = false;

void set_pin_fake(Gpio_Channel_T pin){
    if(CHAN_1 == pin){
        pin1_state = true;
    }
    if(CHAN_2 == pin){
        pin2_state = true;
    }
    if(CHAN_3 == pin){
        pin3_state = true;
    }
    if(CHAN_4 == pin){
        pin4_state = true;
    }
}

void clear_pin_fake(Gpio_Channel_T pin){
    if(CHAN_1 == pin){
        pin1_state = false;
    }
    if(CHAN_2 == pin){
        pin2_state = false;
    }
    if(CHAN_3 == pin){
        pin3_state = false;
    }
    if(CHAN_4 == pin){
        pin4_state = false;
    }
}


TEST_CASE( "Test computing raw ticks", "[PWM]" ) {
    REQUIRE( Convert_Thrust_To_Tics(100) == 200 );
    REQUIRE( Convert_Thrust_To_Tics(0) == 100 );
    REQUIRE( Convert_Thrust_To_Tics(50) == 150 );
}


TEST_CASE( "Set PWM level", "[PWM]" ) {
    Set_Pwm(CHAN_1, 100);
    Set_Pwm(CHAN_2, 90);
    Set_Pwm(CHAN_3, 70);
    Set_Pwm(CHAN_4, 60);

    REQUIRE(Perc_Array[0] == 100);
    REQUIRE(Perc_Array[1] == 90);
    REQUIRE(Perc_Array[2] == 70);
    REQUIRE(Perc_Array[3] == 60);
}

TEST_CASE( "Get PWM level", "[PWM]" ) {
    Perc_Array[0] = 20;
    Perc_Array[1] = 30;
    Perc_Array[2] = 40;
    Perc_Array[3] = 50;

    REQUIRE(Get_Pwm(CHAN_1) == 20);
    REQUIRE(Get_Pwm(CHAN_2) == 30);
    REQUIRE(Get_Pwm(CHAN_3) == 40);
    REQUIRE(Get_Pwm(CHAN_4) == 50);
}

TEST_CASE( "Test PWM main task", "[PWM]" ) {
    Gpio_Interface.set_high = &set_pin_fake;
    Gpio_Interface.set_low = &clear_pin_fake;

    Set_Pwm(CHAN_1, 0);
    Set_Pwm(CHAN_2, 50);
    Set_Pwm(CHAN_3, 70);
    Set_Pwm(CHAN_4, 100);

    Run_Pwm_Blocking();
    REQUIRE( pin1_state == true);
    REQUIRE( pin2_state == true);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<100; a++){
        Run_Pwm_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == true);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<50; a++){
        Run_Pwm_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<20; a++){
        Run_Pwm_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == false);
    REQUIRE( pin4_state == true);

    for(int a=0; a<30; a++){
        Run_Pwm_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == false);
    REQUIRE( pin4_state == false);
}

TEST_CASE( "Test PWM main task - data changed in fly", "[PWM]" ) {
    Gpio_Interface.set_high = &set_pin_fake;
    Gpio_Interface.set_low = &clear_pin_fake;

    Set_Pwm(CHAN_1, 0);

    Run_Pwm_Blocking();
    for(int a=0; a<100; a++){
        Run_Pwm_Blocking();
    }
    REQUIRE( pin1_state == false);

    Set_Pwm(CHAN_1, 50);
    Run_Pwm_Blocking();
 
    REQUIRE( pin1_state == false);
}