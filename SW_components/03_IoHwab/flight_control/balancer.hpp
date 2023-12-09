#pragma once
#include <cstdint>

typedef enum
{
    MOTOR_1,
    MOTOR_2
}
Motor_Id_T;

void *CalculateFlightControls(void *data_ptr);
void *DoMainRoutine(void);

class Balancer
{
private:
    /* data */
    int32_t thrust_ = 0;
public:
    Balancer(/* args */);
    void SetBaseThrust(int32_t thrust);
    int32_t GetCurrentThrust(Motor_Id_T channel) const;
    ~Balancer();
    void ProcessControl(void) const;
};