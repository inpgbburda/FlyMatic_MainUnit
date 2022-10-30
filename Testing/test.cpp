#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "pwm.hpp"

TEST_CASE( "Ticks are computed", "[factorial]" ) {
    REQUIRE( Thrust_To_Tics(100) == 2000 );
    REQUIRE( Thrust_To_Tics(0) == 1000 );
    REQUIRE( Thrust_To_Tics(50) == 1500 );
}