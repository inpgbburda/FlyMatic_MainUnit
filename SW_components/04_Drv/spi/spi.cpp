#include "spi.hpp"
#include <wiringPiSPI.h>

spi::spi(/* args */)
{
}

spi::Init(const int hw_spi)
{
    wiringPiSPISetup(hw_spi, SPI_SPEED);
}

void spi::ReadWriteData(const int channel, unsigned char *buffer, int length)
{
    wiringPiSPIDataRW(channel, buffer, length);
}

spi::~spi()
{
}
