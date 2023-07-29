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
    curr_slave_addr_ = INVALID_SLAVE_ADDR;
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
    curr_slave_addr_ = slave_addr;
    //TODO: Add handling negative result
}

uint8_t I2c::GetCurrentSlaveAddr(void) const
{
    return curr_slave_addr_;
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

int I2c::ReadByte(uint8_t slave_addr, int addr)
{
    int read_val;
    SetSlaveAddr(slave_addr);
    read_val = i2c_smbus_read_byte_data(linux_driver_, addr);
    if (0 > read_val) {
        /* ERROR HANDLING: i2c transaction failed */
        throw std::runtime_error("I2C reading error");
    }
    return read_val;
}

std::vector<uint8_t> I2c::ReadBlockOfBytes(uint8_t start_reg_addr, uint8_t block_len) const
{
    std::vector<uint8_t> Block(block_len);
    i2c_smbus_read_i2c_block_data(linux_driver_, start_reg_addr, block_len, Block.data());
    return Block;
}
I2c::~I2c()
{
}




