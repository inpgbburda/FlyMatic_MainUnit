#include <stdint.h>

int i2c_smbus_read_byte_data(int file, uint8_t command);
int i2c_smbus_write_byte_data(int file, uint8_t command, uint8_t value);
uint32_t i2c_smbus_read_i2c_block_data(int file, uint8_t command, uint8_t length, uint8_t *values);