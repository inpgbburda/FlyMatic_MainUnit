#pragma once
#include <cstdint>

typedef enum
{
    MOTOR_1 = 0,
    MOTOR_2,
    MOTOR_3,
    MOTOR_4,
    MAX_MOTOR_NUM
}
Motor_Id_T;

void *CalculateFlightControls(void *data_ptr);
void *ReadAccSensor(void *data_ptr);
void *DoMainRoutine(void);

class Balancer
{
private:
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
    Balancer(/* args */);
    void SetBaseThrust(uint8_t thrust);
    void SetRegulatorConstants(float kp, float ki, float kd);
    uint8_t GetCurrentThrust(Motor_Id_T channel) const;
    void SetTargetAngle(int32_t angle);
    void ProcessControl(void);
    ~Balancer();
};