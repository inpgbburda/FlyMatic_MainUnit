#include <iostream>
#include "pwm.hpp"
#include "balancer.hpp"
#include <mutex>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/mman.h>

#include "Thread_Manager.hpp"
#ifdef _RASP
#include <wiringPi.h>
#endif /* _RASP */
#include "i2c.hpp"
#include "i2c_cfg.hpp"
#include "pwm.hpp"


extern std::vector<RT_Thread> Initial_Threads_G;
extern Thread_Manager Manager_G;

I2c i2c;

int main()
{
    /* Lock memory - prevent from paging to the swap area -
    * all of the process memory will stay in RAM
    */
    if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) 
    {
        printf("mlockall failed: %m\n");
        exit(-2);
    }
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    i2c.Init();
    Init_Pwm();

    i2c.SetSlaveAddr(I2C_MPU6050_ADD);
    i2c.ReadByte(I2C_MPU6050_WHO_AM_I_REG);
    
    Manager_G.CollectThreads(Initial_Threads_G);
    Manager_G.RunAllThreads();
    DoMainRoutine();

    return 0;
}
