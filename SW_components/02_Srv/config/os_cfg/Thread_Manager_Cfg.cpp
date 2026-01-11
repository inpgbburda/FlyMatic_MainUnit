/**
* Config of the real-time threads used in the system
*
*/

/*
|===================================================================================================================================|
  File includes 
|===================================================================================================================================|
*/

#include "Thread_Manager_Cfg.hpp"
#include "Thread_Manager.hpp"
#include "balancer.hpp"

/*
|===================================================================================================================================|
  Macro definitions
|===================================================================================================================================|
*/

#define MILI_TO_NANOSECONDS(Mili_sec) (( Mili_sec * 1000000U ))

#define FLIGHT_CTRL_RUNTIME_THR     MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_DEADLINE_THR    MILI_TO_NANOSECONDS(20)
#define FLIGHT_CTRL_PERIOD_THR      MILI_TO_NANOSECONDS(100)

#define MPU6050_READ_RUNTIME_THR     MILI_TO_NANOSECONDS(10)
#define MPU6050_READ_DEADLINE_THR    MILI_TO_NANOSECONDS(10)
#define MPU6050_READ_PERIOD_THR      MILI_TO_NANOSECONDS(50)

/*
|===================================================================================================================================|
  Local types definitions 
|===================================================================================================================================|
*/

/*
|===================================================================================================================================|
  Object allocations 
|===================================================================================================================================|
*/

/* The core of the tasks (threads) configuration */
RT_Thread Thr_Flight_Ctrl = RT_Thread(                             \
                                 SCHED_DEADLINE             \
                                ,FLIGHT_CTRL_RUNTIME_THR    \
                                ,FLIGHT_CTRL_DEADLINE_THR   \
                                ,FLIGHT_CTRL_PERIOD_THR     \
                                ,CalculateFlightControls    \
                              );

RT_Thread Thr_Mpu6050_Read = RT_Thread(                             \
                                 SCHED_DEADLINE              \
                                ,MPU6050_READ_RUNTIME_THR    \
                                ,MPU6050_READ_DEADLINE_THR   \
                                ,MPU6050_READ_PERIOD_THR     \
                                ,ReadAccSensor               \
                              );

                              
std::vector<RT_Thread> Initial_Threads = {Thr_Flight_Ctrl, Thr_Mpu6050_Read};

/* End of tasks configuration*/

/*
|===================================================================================================================================|
    Local function declarations
|===================================================================================================================================|
*/

/*
|===================================================================================================================================|
    Function definitions
|===================================================================================================================================|
*/

std::vector<RT_Thread> GetInitialThreadsCfg()
{
    return Initial_Threads;
}
