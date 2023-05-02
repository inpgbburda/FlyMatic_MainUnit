/**
* File contains low layer implementation of I2C bus
*
*/

#pragma once

#ifdef _UNIT_TEST
#define PRIVATE_IFN_UT public:
#else
#define PRIVATE_IFN_UT private:
#endif

void i2c_main_fun(void);
class I2c
{
PRIVATE_IFN_UT
    int file_dptr;
    void ComposeDriverFilename(char* filename, int adapter_nr);
    void OpenDriverFile(char filename[]);

public:
    I2c(/* args */);
    void Init(void);
    void SetSlaveAddr(uint32_t slave_addr);
    int ReadByte(int addr);
    ~I2c();
};

