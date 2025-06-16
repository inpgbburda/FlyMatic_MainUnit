/**
* File contains 
*
*/

/*
|===================================================================================================================================|
    File includes 
|===================================================================================================================================|
*/
#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "mpu6050.hpp"
#include "spi.hpp"

#include <iostream>
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
extern Mpu6050 mpu6050;
spi spi_bus;

const int base = 25;
const float k = 0.03;
const float I = 0.02;

static const int SPI_CHANNEL = 1;
static const int SPI_SPEED = 500000;
unsigned char buffer[100];

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

void *CalculateFlightControls(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    
    int fd, result;
    int32_t angle_x = 0;
    int diff=0;
    static float prev_diff = 0;
    
    sleep(5);
    spi_bus.Init(SPI_CHANNEL, SPI_SPEED);

    while(1)
    {
        int power_1;
        int power_2;
        int control;
                
        mpu6050.ReadAndProcessSensorData();
        angle_x = mpu6050.GetSpiritAngle(ROLL);

        diff = 0 - angle_x;
        prev_diff = prev_diff + (float)diff;
        control  = int((float)diff*(-k) + prev_diff*(-I));
        power_1 = base + control;
        power_2 = base - control;
        std::cout << " roll angle X: " << angle_x <<"; Power 1 " << power_1 << "; Power 2 " << power_2 << std::endl;
        
        buffer[0] = 0x76;
        buffer[1] = 0x75;
        buffer[2] = 0x74;
        buffer[3] = 0x73;
        
        spi_bus.ReadWriteData(SPI_CHANNEL, buffer, 4);
        
        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return NULL;
}

void *DoMainRoutine(void)
{
        std::cout << "Step 1" << std::endl;
        buffer[0] = 0x0;
        buffer[1] = 0x0;
        buffer[2] = 0x0;
        buffer[3] = 0x0;

        spi_bus.ReadWriteData(SPI_CHANNEL, buffer, 4);
        sleep(3);

        sleep(30);
    return NULL;
}

Balancer::Balancer(/* args */)
{
}

void Balancer::SetBaseThrust(int32_t thrust)
{
    thrust_ = thrust;
}

int32_t Balancer::GetCurrentThrust(Motor_Id_T channel) const
{
    return thrust_;
}

Balancer::~Balancer()
{
}

void Balancer::ProcessControl(void) const
{
}



