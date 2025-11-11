#include "spi.hpp"
#include <wiringPiSPI.h>

Spi::Spi(/* args */)
{
}

void Spi::Init(const int hw_spi, int speed)
{
    wiringPiSPISetup(hw_spi, speed);
}

void Spi::ReadWriteData(const int channel, unsigned char *buffer, int length)
{
    wiringPiSPIDataRW(channel, buffer, length);
}

Spi::~Spi()
{
}
