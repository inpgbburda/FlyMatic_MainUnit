#include "Thread_Manager_Cfg.hpp"
#include "Thread_Manager.hpp"
#include "balancer.hpp"

#define MILI_TO_NANOSECONDS(Mili_sec) (( Mili_sec * 1000000U ))

#define FLIGHT_CTRL_RUNTIME_THR     MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_DEADLINE_THR    MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_PERIOD_THR      MILI_TO_NANOSECONDS(100)

/* The core of the tasks (threads) configuration */
RT_Thread Thr_Flight_Ctrl = RT_Thread(                             \
                                 SCHED_DEADLINE             \
                                ,FLIGHT_CTRL_RUNTIME_THR    \
                                ,FLIGHT_CTRL_DEADLINE_THR   \
                                ,FLIGHT_CTRL_PERIOD_THR     \
                                ,CalculateFlightControls    \
                              );

std::vector<RT_Thread> Initial_Threads = {Thr_Flight_Ctrl};

/* End of tasks configuration*/

std::vector<RT_Thread> GetInitialThreads()
{
    return GetInitialThreads();
}
