#pragma once

#include <vector>
#include <stdint.h>

class I2c
{
public:
    int ReadByte(int addr) const;
    std::vector<uint8_t> ReadBlockOfBytes(uint8_t start_reg_addr, uint8_t block_len) const;
};