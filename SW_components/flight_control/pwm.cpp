#include"pwm.hpp"

uint32_t Thrust_To_Tics(int percentage){
    uint32_t tics = 0;
    tics = ((percentage * (MAX_TICS-MIN_TICS)) / 100) + MIN_TICS;
    return tics;
}