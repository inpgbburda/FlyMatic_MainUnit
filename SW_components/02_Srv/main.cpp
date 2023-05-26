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

#define PWR_MGMT_1_REG 0x6B

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

    i2c.Init(DRV_1);
    Init_Pwm();
    i2c.SetSlaveAddr(I2C_MPU6050_ADD);
    i2c.ReadByte(I2C_MPU6050_WHO_AM_I_REG);
    i2c.WriteByte(PWR_MGMT_1_REG, 0x00);

    volatile int x_acc = 0;
    while(1){
        x_acc = i2c.ReadByte(0x3B);
        sleep(1);
        std::cout << "Acceleration is: "<< x_acc << std::endl;

    }
    Manager_G.CollectThreads(Initial_Threads_G);
    Manager_G.RunAllThreads();
    DoMainRoutine();

    return 0;
}
