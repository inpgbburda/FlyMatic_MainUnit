#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "i2c.hpp"

int I2c::ReadByte(uint8_t slave_addr, uint8_t addr)
{
    return mock().actualCall("ReadByte")
                .withParameter("slave_addr", slave_addr)
                .withParameter("addr", addr)
                .returnIntValue();
}               

void I2c:: WriteByte(uint8_t slave_addr, int addr, uint8_t data)
{
    mock().actualCall("WriteByte")
        .withParameter("slave_addr", slave_addr)
        .withParameter("addr", addr)
        .withParameter("data", data);
}

std::vector<uint8_t> I2c::ReadBlockOfBytes(uint8_t slave_addr, uint8_t start_reg_addr, uint8_t block_len)
{
    auto Block_Of_Bytes = mock().actualCall("ReadBlockOfBytes")
                                .withParameter("slave_addr", slave_addr)
                                .withParameter("start_reg_addr", start_reg_addr)
                                .withParameter("block_len", block_len)
                                .returnPointerValue();
    return *static_cast<std::vector<uint8_t>*>(Block_Of_Bytes);
}