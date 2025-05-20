#include "Thread_Manager_Cfg.hpp"
#include "balancer.hpp"

#define MILI_TO_NANOSECONDS(Mili_sec) (( Mili_sec * 1000000U ))

#define FLIGHT_CTRL_RUNTIME_THR     MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_DEADLINE_THR    MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_PERIOD_THR      MILI_TO_NANOSECONDS(100)

RT_Thread thread_2 = RT_Thread(                             \
                                 SCHED_DEADLINE             \
                                ,FLIGHT_CTRL_RUNTIME_THR    \
                                ,FLIGHT_CTRL_DEADLINE_THR   \
                                ,FLIGHT_CTRL_PERIOD_THR     \
                                ,CalculateFlightControls    \
                              );

std::vector<RT_Thread> Initial_Threads_G = {thread_2};
Thread_Manager Manager_G = Thread_Manager();
