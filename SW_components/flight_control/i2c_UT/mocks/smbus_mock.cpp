extern "C" {
  #include <i2c/smbus.h>
}
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

int i2c_smbus_read_byte_data(int I2c_File_Dcr,int reg_address)
{
    mock().actualCall("i2c_smbus_read_byte_data");
}