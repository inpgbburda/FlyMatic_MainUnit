#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "i2c.hpp"

int I2c::ReadByte(int addr) const
{
    return mock().actualCall("ReadByte").returnIntValue();
}

std::vector<uint8_t> I2c::ReadBlockOfBytes(uint8_t start_reg_addr, uint8_t block_len) const
{
    auto Block_Of_Bytes = mock().actualCall("ReadBlockOfBytes").returnPointerValue();
    return *static_cast<std::vector<uint8_t>*>(Block_Of_Bytes);
}