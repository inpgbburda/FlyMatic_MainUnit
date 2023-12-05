#pragma once

void *CalculateFlightControls(void *data_ptr);

void *DoMainRoutine(void);

class Balancer
{
private:
    /* data */
public:
    Balancer(/* args */);
    ~Balancer();
    void ProcessControl(void) const;
};