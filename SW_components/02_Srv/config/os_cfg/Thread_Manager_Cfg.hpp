#pragma once

#include "Thread_Manager.hpp"
#include <vector>

#define THR_MNGR_FLIGHT_CTRL 0U
#define THR_MNGR_MPU6050_READ 1U

/**
 * GetInitialThreadsCfg
 * @brief: Returns a vector of RT_Thread objects with initial thread configurations
 * @param: none
 *
 * @return: vector of RT_Thread objects with initial thread configurations
 * 
 */
std::vector<RT_Thread> GetInitialThreadsCfg();