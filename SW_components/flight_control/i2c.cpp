#include "i2c.hpp"

  #include <linux/i2c.h>
  #include <linux/i2c-dev.h>
  #include <i2c/smbus.h>
 
 
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
 
#include <cstdio>
#include <sys/ioctl.h>
#include "i2c.hpp"

void i2c_main_fun(void)
{
   printf("Hello I2c swiry\n");
   int file = 0;
   int adapter_nr = 1; /* probably dynamically determined */
   char filename[20];
   
   snprintf(filename, 19, "/dev/i2c-%d", adapter_nr); /* (buffer to safe string; max buffer size in bytes; filename with var <d>; var) */
   file = open(filename, O_RDWR); /* Open file read/write access*/
   if (file < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      printf("Failed to open file\n");
   }
   
   int addr = 0x30; /* The I2C address */
   
   if (ioctl(file, I2C_SLAVE, addr) < 0) {
      /* ERROR HANDLING; you can check errno to see what went wrong */
      printf("Failed to ioctl\n");
   }
   
   __u8 reg = 0x10; /* Device register to access */
   __s32 res;
   char buf[10];
   

   buf[0] = reg;
   buf[1] = 0x43;
   buf[2] = 0x65;
   if (write(file, buf, 3) != 3) {
      /* ERROR HANDLING: i2c transaction failed */
      printf("Failed to write data\n");
   }
}


