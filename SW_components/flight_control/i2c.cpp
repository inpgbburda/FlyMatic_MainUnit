extern "C" {
  #include <linux/i2c-dev.h>
  #include <i2c/smbus.h>
}


#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#include <cstdio>
#include <sys/ioctl.h>


#include <stdint.h>
#include "i2c_cfg.hpp"

static int I2c_File_Dcr = 0;

static uint8_t I2c_Read_Byte(uint8_t reg_address);
static uint8_t ComposeDriverFilename(char* filename, int adapter_nr);

void i2c_main_fun(void)
{
    int adapter_nr = I2C_KERNEL_ADAPTER_NUM; /* probably dynamically determined */
    char filename[I2C_KERNEL_FILENAME_MAX_SIZE];


    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); /* (buffer to safe string; max buffer size in bytes; filename with var <d>; var) */
    I2c_File_Dcr = open(filename, O_RDWR); /* Open file read/write access*/
    if (I2c_File_Dcr < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("Failed to open file\n");
    }

    int addr = I2C_MPU6050_ADD;


    if (ioctl(I2c_File_Dcr, I2C_SLAVE, addr) < 0) {
        /* ERROR HANDLING; you can check errno to see what went wrong */
        printf("Failed to ioctl\n");
    }


    uint8_t reg = I2C_MPU6050_WHO_AM_I_REG;
    uint8_t res = I2c_Read_Byte(reg);
    printf("Read slave address is: 0x%02X\n", res);
    
}

static uint8_t ComposeDriverFilename(char* filename, int adapter_nr)
{
    return -1;
}

static uint8_t I2c_Read_Byte(uint8_t reg_address)
{
  int32_t result;
  uint8_t read_data;
  result = i2c_smbus_read_byte_data(I2c_File_Dcr, reg_address);
  if (result < 0) {
     /* ERROR HANDLING: i2c transaction failed */
     read_data = 0U;
  } else {
     read_data = (uint8_t)result;
  }
  return result;
}



