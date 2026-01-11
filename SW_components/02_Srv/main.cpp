/**
* Main application file
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/

#include <iostream>

#include "Thread_Manager.hpp"
#include "i2c.hpp"
#include "spi.hpp"
#include "i2c_cfg.hpp"
#include "mpu6050.hpp"
#include "balancer.hpp"

#include "Thread_Manager_Cfg.hpp"

/*
|===================================================================================================================================|
    Macro definitions
|===================================================================================================================================|
*/

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
static const int SPI_CHANNEL = 0;
static const int SPI_SPEED = 500000;

static I2c I2c_Bus;
static Spi Spi_Bus;
static Mpu6050 Mpu6050_Inst = Mpu6050(&I2c_Bus);
static Balancer Balancer_Inst = Balancer(Mpu6050_Inst, Spi_Bus, SPI_CHANNEL);

static Thread_Manager thr_manager = Thread_Manager();

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

int main()
{
    std::cout << "Witam serdecznie w projekcie drona"<< std::endl;

    I2c_Bus.Init(DRV_1);
    Spi_Bus.Init(SPI_CHANNEL, SPI_SPEED);

    Mpu6050_Inst.Start();
    Mpu6050_Inst.SetLowPassFilter(LEVEL_5);
    auto init_threads = GetInitialThreadsCfg();

    init_threads[THR_MNGR_FLIGHT_CTRL].SetUserArg((void*)&Balancer_Inst);
    init_threads[THR_MNGR_MPU6050_READ].SetUserArg((void*)&Mpu6050_Inst);

    thr_manager.Init(init_threads);
    thr_manager.RunAllThreads();

    DoMainRoutine(Balancer_Inst);

    return 0;
}
