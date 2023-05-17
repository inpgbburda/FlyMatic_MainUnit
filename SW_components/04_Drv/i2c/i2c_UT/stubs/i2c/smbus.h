#include <stdint.h>

int i2c_smbus_read_byte_data(int I2c_File_Dcr, int reg_address);
int i2c_smbus_write_byte_data(int I2c_File_Dcr, int reg_address, uint8_t data);
