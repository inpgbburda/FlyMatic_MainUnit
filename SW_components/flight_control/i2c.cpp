extern "C" {
  #include <linux/i2c-dev.h>
#ifndef _UNIT_TEST 
  #include <i2c/smbus.h>
#endif /* _UNIT_TEST */
}

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#include <cstdio>
#include <sys/ioctl.h>


#include <stdint.h>
#include <stdexcept>
#include "i2c_cfg.hpp"

#define I2C_DRV_DESRC_MAX_FILE_L 20

static int I2c_File_Dcr = 0;

static uint8_t I2c_Read_Byte(uint8_t reg_address);
static void ComposeDriverFilename(char* filename, int adapter_nr);
// static void SetSlaveAddr();
static void DriverInit(int driver_num);

void i2c_main_fun(void)
{
    int adapter_nr = I2C_KERNEL_ADAPTER_NUM; /* probably dynamically determined */
    char filename[I2C_KERNEL_FILENAME_MAX_SIZE];

    ComposeDriverFilename(filename, adapter_nr);
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


static uint8_t I2c_Read_Byte(uint8_t reg_address)
{
  int32_t result;
  uint8_t read_data;
#ifndef _UNIT_TEST 
  result = i2c_smbus_read_byte_data(I2c_File_Dcr, reg_address);
#endif
  if (result < 0) {
     /* ERROR HANDLING: i2c transaction failed */
     read_data = 0U;
  } else {
     read_data = (uint8_t)result;
  }
  return result;
}

static void ComposeDriverFilename(char* filename, int adapter_nr)
{
    if( (nullptr != filename) && (adapter_nr >= 0) && (adapter_nr < 10) )
    {
        snprintf(filename, I2C_DRV_DESRC_MAX_FILE_L, "/dev/i2c-%d", adapter_nr);
    }
    else
    {
        throw "out of range";
    }
}

static void DriverInit(int driver_num)
{
    
}
