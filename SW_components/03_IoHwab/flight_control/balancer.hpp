#pragma once
#include <cstdint>
#include "spi.hpp"
#include "mpu6050.hpp"

typedef enum
{
    MOTOR_1 = 0,
    MOTOR_2,
    MOTOR_3,
    MOTOR_4,
    MAX_MOTOR_NUM
}
Motor_Id_T;

class Balancer
{
private:
    Mpu6050& mpu6050_;
    Spi& spi_;
    int spi_channel_ = 0;
    /* data */
    uint32_t base_thrust_ = 0U;
    uint8_t thrust_1_ = 0U;
    uint8_t thrust_2_ = 0U;
    int32_t target_angle_ = 0;
    float kp_ = 0.0f;
    float ki_ = 0.0f;
    float kd_ = 0.0f;
    float error_i_ = 0.0f;
    float error_prev_ = 0.0f;
    
public:
    Balancer(Mpu6050& mpu6050, Spi& spi, int spi_channel);
    void Init(void);
    void SetBaseThrust(uint8_t thrust);
    void SetRegulatorConstants(float kp, float ki, float kd);
    uint8_t GetCurrentThrust(Motor_Id_T channel) const;
    void SetTargetAngle(int32_t angle);
    void ProcessControl(void);
    ~Balancer();
};

void *CalculateFlightControls(void *data_ptr);
void *ReadAccSensor(void *data_ptr);
void *DoMainRoutine(Balancer& balancer);