extern "C" {
  #include <linux/i2c-dev.h>
  #include <i2c/smbus.h>
}

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#include <cstdio>
#include <sys/ioctl.h>


#include <stdint.h>
#include <stdexcept>
#include "i2c_cfg.hpp"
#include "i2c.hpp"

#define I2C_DRV_DESRC_MAX_FILE_L 20

static int I2c_File_Dcr = 0;

static uint8_t I2c_Read_Byte(uint8_t reg_address);

I2c::I2c(/* args */)
{
}

void I2c::ComposeDriverFilename(char* filename, int adapter_nr)
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

void I2c::OpenDriverFile(char filename[])
{
    this->file_dptr = open(filename, O_RDWR);
    //TODO: Add handling negative result
}

void I2c::SetSlaveAddr(uint32_t slave_addr)
{
    ioctl(this->file_dptr, I2C_SLAVE, slave_addr);
    //TODO: Add handling negative result
}

void I2c::Init(void)
{
    char filename[I2C_DRV_DESRC_MAX_FILE_L];
    this->ComposeDriverFilename(filename, 1);
    this->OpenDriverFile(filename);
}

int I2c::ReadByte(int addr)
{
    int dummy_dtr = 0;
    int read_val;
    read_val = i2c_smbus_read_byte_data(this->file_dptr, addr);
    if (0 > read_val) {
        /* ERROR HANDLING: i2c transaction failed */
        throw std::runtime_error("I2C reading error");
    }
    return read_val;
}

I2c::~I2c()
{
}




