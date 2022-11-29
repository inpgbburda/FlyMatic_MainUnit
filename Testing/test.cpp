#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "pwm.hpp"


static bool pin1_state = false;
static bool pin2_state = false;
static bool pin3_state = false;
static bool pin4_state = false;

void set_pin_fake(int pin){
    if(1 == pin){
        pin1_state = true;
    }
    if(2 == pin){
        pin2_state = true;
    }
    if(3 == pin){
        pin3_state = true;
    }
    if(4 == pin){
        pin4_state = true;
    }
}

void clear_pin_fake(int pin){
    if(1 == pin){
        pin1_state = false;
    }
    if(2 == pin){
        pin2_state = false;
    }
    if(3 == pin){
        pin3_state = false;
    }
    if(4 == pin){
        pin4_state = false;
    }
}

extern GPIO_Interface_T Gpio_Interface;

TEST_CASE( "Test computing raw ticks", "[PWM]" ) {
    REQUIRE( Thrust_To_Tics(100) == 200 );
    REQUIRE( Thrust_To_Tics(0) == 100 );
    REQUIRE( Thrust_To_Tics(50) == 150 );
}

extern uint32_t Perc1;
extern uint32_t Perc2;
extern uint32_t Perc3;
extern uint32_t Perc4;

TEST_CASE( "Set PWM level", "[PWM]" ) {
    Set_PWM(CHAN_1, 100);
    Set_PWM(CHAN_2, 90);
    Set_PWM(CHAN_3, 70);
    Set_PWM(CHAN_4, 60);

    REQUIRE(Perc1 == 100);
    REQUIRE(Perc2 == 90);
    REQUIRE(Perc3 == 70);
    REQUIRE(Perc4 == 60);
}

TEST_CASE( "Test PWM main task", "[PWM]" ) {
    Gpio_Interface.set_high = &set_pin_fake;
    Gpio_Interface.set_low = &clear_pin_fake;

    Set_PWM(CHAN_1, 0);
    Set_PWM(CHAN_2, 50);
    Set_PWM(CHAN_3, 70);
    Set_PWM(CHAN_4, 100);

    Run_PWM_Blocking();
    REQUIRE( pin1_state == true);
    REQUIRE( pin2_state == true);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<100; a++){
        Run_PWM_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == true);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<50; a++){
        Run_PWM_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<20; a++){
        Run_PWM_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == false);
    REQUIRE( pin4_state == true);

    for(int a=0; a<30; a++){
        Run_PWM_Blocking();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == false);
    REQUIRE( pin4_state == false);
}