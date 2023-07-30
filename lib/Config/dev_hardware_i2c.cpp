/*****************************************************************************
This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/blob/master/RaspberryPi/user_dev/c/lib/Config/DEV_Config.c#L104
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#include "dev_hardware_i2c.hpp"
#include <cstdio>
#include <cstdio>
#include <cstdlib>   //exit()  
extern "C" {
	#include <fcntl.h>    //define O_RDWR  
	#include <linux/i2c-dev.h>  
	#include <sys/ioctl.h>
	#include <unistd.h>
}

HARDWARE_I2C hardware_i2c;
using namespace std;
/******************************************************************************
function: I2C device initialization
parameter:
    i2c_device : Device name
Info:   /dev/i2c-*
******************************************************************************/
void DEV_I2C::DEV_HARDWARE_I2C_begin(char *i2c_device)
{
    //device
    if((hardware_i2c.fd = open(i2c_device, O_RDWR)) < 0)  { //Turn on I2C
        std::cerr << "Failed to open i2c device." << std::endl;
        std::cout << "Failed to open i2c device" << std::endl;
        std::exit(1); 
    } else {
        DEV_HARDWARE_I2C_Debug("open : %s\r\n", i2c_device);
    }
}

/******************************************************************************
function: I2C device End
parameter:
Info:
******************************************************************************/
void DEV_I2C::DEV_HARDWARE_I2C_end(void)
{
    if (close(hardware_i2c.fd) != 0){
		std::cerr << "Failed to close i2c device." << std::endl;
    }
}

/******************************************************************************
function: Set the device address for I2C access
parameter:
    addr : Device address accessed by I2C
Info:
******************************************************************************/
void DEV_I2C::DEV_HARDWARE_I2C_setSlaveAddress(uint8_t addr)
{
    if(ioctl(hardware_i2c.fd, I2C_SLAVE, addr) < 0)  {  
        std::cerr << "Failed to access bus." << std::endl;
        std::exit(1);  
    }
}

/******************************************************************************
function:   I2C Send data
parameter:
    buf  : Send data buffer address
    len  : Send data length
Info:
******************************************************************************/
uint8_t DEV_I2C::DEV_HARDWARE_I2C_write(const char * buf, uint32_t len)
{
    write(hardware_i2c.fd, buf, len);
    return 0;
}

/******************************************************************************
function:   I2C read data
parameter:
    reg  : Read data register address
    buf  : Sead data buffer address
    len  : Sead data length
Info:
******************************************************************************/
uint8_t DEV_I2C::DEV_HARDWARE_I2C_read(uint8_t reg, char* buf, uint32_t len)
{
    uint8_t temp[1] = {reg};
    write(hardware_i2c.fd, temp, 1); 
    read(hardware_i2c.fd, buf, len);
    return 0;
}