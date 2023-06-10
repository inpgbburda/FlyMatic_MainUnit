extern "C" {
  #include <i2c/smbus.h>
}
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

int i2c_smbus_read_byte_data(int file, uint8_t command)
{
    mock().actualCall("i2c_smbus_read_byte_data");
    return mock().intReturnValue();
}

int i2c_smbus_write_byte_data(int file, uint8_t command, uint8_t value)
{
    mock().actualCall("i2c_smbus_write_byte_data");
    return mock().intReturnValue();
}

uint32_t i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length, uint8_t *values)
{
    mock().actualCall("i2c_smbus_read_i2c_block_data")
        .withParameter("file", file)
        .withParameter("command", command)
        .withParameter("length", length)
        .withOutputParameter("values", values);

    return mock().intReturnValue();
}