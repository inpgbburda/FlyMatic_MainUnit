#pragma once

#define SPI_SPEED 500000 // 500 kHz

class spi
{
private:
    /* data */
public:
    spi(/* args */);
    void Init(const int hw_spi);
    void ReadWriteData(const int channel, unsigned char *buffer, int length);
    ~spi();
};