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
    void ComposeDriverFilename(char* filename, int adapter_nr);
    friend class TEST_GROUP_CppUTestGroupI2c;
    /* data */
public:
    I2c(/* args */);
    ~I2c();
};

