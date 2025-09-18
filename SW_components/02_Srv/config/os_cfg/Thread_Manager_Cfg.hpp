#pragma once

#include "Thread_Manager.hpp"
#include <vector>

/**
 * GetInitialThreads
 * @brief: Returns a vector of RT_Thread objects with initial thread configurations
 * @param: none
 *
 * @return: vector of RT_Thread objects with initial thread configurations
 * 
 */
std::vector<RT_Thread> GetInitialThreads();