#pragma once

#include <vector>
#include <stdint.h>

class I2c
{
public:
    int ReadByte(uint8_t slave_addr, uint8_t addr);
    void WriteByte(uint8_t slave_addr, int addr, uint8_t data);
    std::vector<uint8_t> ReadBlockOfBytes(uint8_t start_reg_addr, uint8_t block_len) const;
};