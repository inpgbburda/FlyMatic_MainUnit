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


TEST_CASE( "Test computing raw ticks", "[PWM" ) {
    REQUIRE( Thrust_To_Tics(100) == 2000 );
    REQUIRE( Thrust_To_Tics(0) == 1000 );
    REQUIRE( Thrust_To_Tics(50) == 1500 );
}

TEST_CASE( "Test PWM main task", "[PWM]" ) {
    Gpio_Interface.set_high = &set_pin_fake;
    Gpio_Interface.set_low = &clear_pin_fake;
    Run_PWM();
    REQUIRE( pin1_state == true);
    REQUIRE( pin2_state == true);
    REQUIRE( pin3_state == true);
    REQUIRE( pin4_state == true);

    for(int a=0; a<1000; a++){
        Run_PWM();
    }
    REQUIRE( pin1_state == false);
    REQUIRE( pin2_state == false);
    REQUIRE( pin3_state == false);
    REQUIRE( pin4_state == false);

}