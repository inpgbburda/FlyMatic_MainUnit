/**
* File contains low layer configuration of I2C bus
*
*/

#pragma once

#define I2C_KERNEL_ADAPTER_NUM 1
#define I2C_KERNEL_FILENAME_MAX_SIZE 20

/*MPU6050 address is b110100X; X - level on AD0 pin*/
#define I2C_MPU6050_ADD 0x68