#include "Thread_Manager_Cfg.hpp"
#include "pwm.hpp"
#include "balancer.hpp"


RT_Thread thread_1 = RT_Thread(SCHED_DEADLINE, 20000000, 20000000, 20000000, DoPwm);
RT_Thread thread_2 = RT_Thread(SCHED_DEADLINE, 1000000, 20000000, 20000000, CalculateFlightControls);

std::vector<RT_Thread> Initial_Threads_G = {thread_1, thread_2};
Thread_Manager Manager_G = Thread_Manager();
