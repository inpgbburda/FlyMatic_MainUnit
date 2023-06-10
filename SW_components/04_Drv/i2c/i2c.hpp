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

#include <vector>
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

class I2c
{
PRIVATE_IFN_UT
    int linux_driver_;
    std::string filename_;
    std::string ComposeDriverFilename(int adapter_nr) const;
    void OpenDriver(void); //TODO: make dependences unclassified

public:
    I2c(/* args */);
    void Init(const Drv_Instance_T hw_i2c);
    void SetSlaveAddr(uint32_t slave_addr);
    void WriteByte(int addr, uint8_t data);
    int ReadByte(int addr) const;
    std::vector<uint8_t> ReadBlockOfBytes(uint8_t start_reg_addr, uint8_t block_len) const;

    ~I2c();
};

