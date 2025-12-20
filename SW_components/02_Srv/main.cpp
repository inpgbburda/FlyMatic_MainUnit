#include <iostream>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "spi.hpp"
#include "i2c_cfg.hpp"
#include "balancer.hpp"
#include "mpu6050.hpp"

#include "Thread_Manager_Cfg.hpp"

static const int SPI_CHANNEL = 0;
static const int SPI_SPEED = 500000;

I2c i2c;
Spi Spi_Bus;
Mpu6050 mpu6050(&i2c);
Balancer balancer = Balancer(Spi_Bus, SPI_CHANNEL);

Thread_Manager thr_manager = Thread_Manager();

int main()
{
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    i2c.Init(DRV_1);
    Spi_Bus.Init(SPI_CHANNEL, SPI_SPEED);

    mpu6050.Start();
    mpu6050.SetLowPassFilter(LEVEL_5);
    auto init_threads = GetInitialThreads();

    init_threads[THR_MNGR_FLIGHT_CTRL].SetUserArg((void*)&balancer); // Flight control thread
    init_threads[THR_MNGR_MPU6050_READ].SetUserArg((void*)nullptr); // MPU6050 read

    thr_manager.Init(init_threads);
    thr_manager.RunAllThreads();

    DoMainRoutine(balancer);

    return 0;
}
