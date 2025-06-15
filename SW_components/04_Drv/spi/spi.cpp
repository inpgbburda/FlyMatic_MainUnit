#include "spi.hpp"
#include <wiringPiSPI.h>

spi::spi(/* args */)
{
}

void spi::Init(const int hw_spi, int speed)
{
    wiringPiSPISetup(hw_spi, speed);
}

void spi::ReadWriteData(const int channel, unsigned char *buffer, int length)
{
    wiringPiSPIDataRW(channel, buffer, length);
}

spi::~spi()
{
}
