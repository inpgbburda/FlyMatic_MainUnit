#include <iostream>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "i2c_cfg.hpp"
#include "balancer.hpp"
#include "mpu6050.hpp"

I2c i2c;
Mpu6050 mpu6050(&i2c);

int main()
{
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    i2c.Init(DRV_1);

    mpu6050.Start();
    mpu6050.SetLowPassFilter(LEVEL_5);
    
    Manager_G.CollectThreads(Initial_Threads_G);
    Manager_G.RunAllThreads();
    DoMainRoutine();

    return 0;
}
