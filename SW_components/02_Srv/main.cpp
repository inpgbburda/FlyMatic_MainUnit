#include <iostream>
#include <sys/mman.h>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "i2c_cfg.hpp"
#include "pwm.hpp"
#include "balancer.hpp"
#include "mpu6050.hpp"

extern std::vector<RT_Thread> Initial_Threads_G;
extern Thread_Manager Manager_G;

I2c i2c;
Mpu6050 mpu6050;

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

    Init_Pwm();
    i2c.Init(DRV_1);
    i2c.SetSlaveAddr(I2C_MPU6050_ADD);
    mpu6050.Init(&i2c);

    mpu6050.Start();
    
    Manager_G.CollectThreads(Initial_Threads_G);
    Manager_G.RunAllThreads();
    DoMainRoutine();

    return 0;
}
