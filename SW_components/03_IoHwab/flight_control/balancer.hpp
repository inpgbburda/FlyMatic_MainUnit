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
    int32_t thrust_ = 0;
    int32_t target_angle_ = 0;
public:
    Balancer(/* args */);
    void SetBaseThrust(int32_t thrust);
    int32_t GetCurrentThrust(Motor_Id_T channel) const;
    void SetTargetAngle(int32_t angle);
    void ProcessControl(void) const;
    ~Balancer();
};