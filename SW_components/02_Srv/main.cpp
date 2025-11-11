#include <iostream>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "i2c_cfg.hpp"
#include "balancer.hpp"
#include "mpu6050.hpp"

#include "Thread_Manager_Cfg.hpp"


I2c i2c;
Mpu6050 mpu6050(&i2c);
Thread_Manager thr_manager = Thread_Manager();

int main()
{
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    i2c.Init(DRV_1);

    mpu6050.Start();
    mpu6050.SetLowPassFilter(LEVEL_5);
    auto init_threads = GetInitialThreads();

    thr_manager.Init(init_threads);
    thr_manager.RunAllThreads();

    DoMainRoutine();

    return 0;
}
