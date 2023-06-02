#include <stdint.h>
//TODO: check if arguments shouldnt have identical names with raspberry linux api
int i2c_smbus_read_byte_data(int I2c_File_Dcr, int reg_address);
int i2c_smbus_write_byte_data(int I2c_File_Dcr, int reg_address, uint8_t data);
uint32_t i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length, uint8_t *values);