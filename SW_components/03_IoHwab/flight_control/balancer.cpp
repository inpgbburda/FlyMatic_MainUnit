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
Spi spi_bus;

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
    
    int32_t angle_x = 0;
    int diff=0;
    static float prev_diff = 0;
    
    spi_bus.Init(SPI_CHANNEL, SPI_SPEED);

    while(1)
    {
        int power_1;
        int power_2;
        int control;
                
        mpu6050.ProcessSensorData();
        angle_x = mpu6050.GetSpiritAngle(ROLL);

        diff = 0 - angle_x;
        prev_diff = prev_diff + (float)diff;
        control  = int((float)diff*(-k) + prev_diff*(-I));
        power_1 = base + control;
        power_2 = base - control;
        std::cout << " roll angle X: " << angle_x <<"; Power 1 " << power_1 << "; Power 2 " << power_2 << std::endl;
        
        buffer[0] = 0x76; /* Proppeller 1 */
        buffer[1] = 0x75; /* Proppeller 2 */
        buffer[2] = 0x74;
        buffer[3] = 0x73;
        
        spi_bus.ReadWriteData(SPI_CHANNEL, buffer, 4);
        
        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return NULL;
}

void *ReadAccSensor(void *data_ptr)
{
    SchedSetAttr((sched_attr_t*)data_ptr);
    while(1)
    {
        mpu6050.ReadSensorData();
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
    Spi spi;
    uint8_t spi_buffer[MAX_MOTOR_NUM] = {0};
    int32_t roll_angle = 0;
    int32_t u = 0;
    int32_t error = 0;
    int32_t thr_1 = 0;
    int32_t thr_2 = 0;
    const int32_t k = 1;

    roll_angle = mpu6050.GetSpiritAngle(ROLL);
    error = target_angle_ - roll_angle;
    u = k * error;

    thr_1 = thrust_ + u;
    thr_2 = thrust_ - u;

    spi_buffer[MOTOR_1] = (thr_1 > 0) ? static_cast<uint8_t>(thr_1) : 0;
    spi_buffer[MOTOR_2] = (thr_2 > 0) ? static_cast<uint8_t>(thr_2) : 0;
    
    spi.ReadWriteData(SPI_CHANNEL, spi_buffer, sizeof(spi_buffer));
}

void Balancer::SetTargetAngle(int32_t angle)
{
    target_angle_ = angle;
}


