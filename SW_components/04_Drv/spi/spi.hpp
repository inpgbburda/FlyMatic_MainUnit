/**
* Spi main header file
*
*/
#pragma once

/*
|===================================================================================================================================|
Exported types declarations
|===================================================================================================================================|
*/

class Spi
{
private:
    /* data */
public:
    Spi(/* args */);
    void Init(const int hw_spi, int speed);
    void ReadWriteData(const int channel, unsigned char *buffer, int length);
    ~Spi();
};