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

//TODO: Move to separate file:
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
int open(const char *__path, int __oflag, ...)
{
    mock().actualCall("open").withParameter("__path", __path).withParameter("__oflag", __oflag);
    return mock().intReturnValue();
}

typedef enum
{
    DRV_1 = 1,
    DRV_2,
    DRV_3,
    DRV_4,
    DRV_5,
    DRV_6,
    DRV_7,
    DRV_8,
    DRV_9
} Drv_Instance_T;

void i2c_main_fun(void);
class I2c
{
PRIVATE_IFN_UT
    int linux_driver_;
    bool inited_;
    std::string filename_;
    std::string ComposeDriverFilename(int adapter_nr) const;
    void OpenDriver(void);

public:
    I2c(/* args */);
    //Drv_Instance_T hw_i2c
    void Init(const Drv_Instance_T hw_i2c);
    bool isInited(void) const;
    void SetSlaveAddr(uint32_t slave_addr);
    void WriteByte(int addr, uint8_t data);
    int ReadByte(int addr);
    ~I2c();
};

