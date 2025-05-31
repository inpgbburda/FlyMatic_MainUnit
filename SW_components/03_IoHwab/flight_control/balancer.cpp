#include "balancer.hpp"
#include "Thread_Manager.hpp"
#include "mpu6050.hpp"

#include <wiringPiSPI.h>

extern uint32_t Time_Calibration_G;
extern Mpu6050 mpu6050;
pthread_mutex_t Pwm_lock_G;
static const int CHANNEL = 1;

void *CalculateFlightControls(void *data_ptr)
{
    int fd, result;
    unsigned char buffer[100];
    SchedSetAttr((sched_attr_t*)data_ptr);

    int32_t angle_x = 0;
    
    while(1)
    {
        // Configure the interface.
        // CHANNEL insicates chip select,
        // 500000 indicates bus speed.
        fd = wiringPiSPISetup(CHANNEL, 500000);

        buffer[0] = 0x76;
        buffer[1] = 0x75;
        buffer[2] = 0x74;
        buffer[3] = 0x73;

        wiringPiSPIDataRW(CHANNEL, buffer, 4);

        mpu6050.ReadAndProcessSensorData();
        angle_x = mpu6050.GetSpiritAngle(ROLL);
        std::cout << " roll angle X: " << angle_x << std::endl;

        /*Inform scheduler that calculation is done*/
        sched_yield();
    }
    return NULL;
}

void *DoMainRoutine(void)
{
        Pwm_lock_G = PTHREAD_MUTEX_INITIALIZER;
        std::cout << "Step 1" << std::endl;
        sleep(5);

        std::cout << "Step 2" << std::endl;
        sleep(5);

        std::cout << "Step 3" << std::endl;
        sleep(5);

        std::cout << "Step 4" << std::endl;
        sleep(1000);
        pthread_mutex_destroy(&Pwm_lock_G);
    return NULL;
}
