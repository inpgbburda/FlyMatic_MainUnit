#include <iostream>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "spi.hpp"
#include "i2c_cfg.hpp"
#include "mpu6050.hpp"
#include "balancer.hpp"

#include "Thread_Manager_Cfg.hpp"

static const int SPI_CHANNEL = 0;
static const int SPI_SPEED = 500000;

static I2c i2c_bus;
static Spi spi_bus;
static Mpu6050 mpu6050 = Mpu6050(&i2c_bus);
static Balancer balancer = Balancer(mpu6050, spi_bus, SPI_CHANNEL);

static Thread_Manager thr_manager = Thread_Manager();

int main()
{
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    i2c_bus.Init(DRV_1);
    spi_bus.Init(SPI_CHANNEL, SPI_SPEED);

    mpu6050.Start();
    mpu6050.SetLowPassFilter(LEVEL_5);
    auto init_threads = GetInitialThreadsCfg();

    init_threads[THR_MNGR_FLIGHT_CTRL].SetUserArg((void*)&balancer);
    init_threads[THR_MNGR_MPU6050_READ].SetUserArg((void*)&mpu6050);

    thr_manager.Init(init_threads);
    thr_manager.RunAllThreads();

    DoMainRoutine(balancer);

    return 0;
}
