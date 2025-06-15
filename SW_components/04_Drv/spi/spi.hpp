#pragma once

class spi
{
private:
    /* data */
public:
    spi(/* args */);
    void Init(const int hw_spi, int speed);
    void ReadWriteData(const int channel, unsigned char *buffer, int length);
    ~spi();
};