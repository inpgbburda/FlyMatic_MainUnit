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


I2c::I2c(/* args */)
{
}

std::string I2c::ComposeDriverFilename(int adapter_nr) const
{
    std::string base = "/dev/i2c-";
    return base + std::to_string(adapter_nr);
}

void I2c::OpenDriver()
{
    linux_driver_ = open(filename_.c_str(), O_RDWR);
    //TODO: Add handling negative result
}

void I2c::SetSlaveAddr(uint32_t slave_addr)
{
    ioctl(linux_driver_, I2C_SLAVE, slave_addr);
    //TODO: Add handling negative result
}

void I2c::WriteByte(int addr, uint8_t data)
{
    i2c_smbus_write_byte_data(linux_driver_, addr, data);
}

void I2c::Init(const Drv_Instance_T hw_i2c)
{
    filename_ = ComposeDriverFilename(hw_i2c);
    OpenDriver();
}

int I2c::ReadByte(int addr)
{
    int read_val;
    read_val = i2c_smbus_read_byte_data(linux_driver_, addr);
    if (0 > read_val) {
        /* ERROR HANDLING: i2c transaction failed */
        throw std::runtime_error("I2C reading error");
    }
    return read_val;
}

std::vector<uint8_t> I2c::ReadBytes(int addr, int len)
{
    std::vector<uint8_t> vector = {1, 5, 16, 8};
    uint8_t* data;
    i2c_smbus_read_i2c_block_data(linux_driver_, addr, 4, data);
    return vector;
}
I2c::~I2c()
{
}




