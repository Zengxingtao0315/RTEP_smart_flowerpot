/*****************************************************************************
* | File        :   dev_hardware_i2c.c
* | Author      :   Xingtao Zeng
* | Function    :   Read and write /dev/i2C,  hardware I2C
* | Info        :

******************************************************************************/
#include "dev_hardware_i2c.h"

#include <stdio.h>
#include <stdlib.h>   //exit()  
#include <fcntl.h>    //define O_RDWR  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>


HARDWARE_I2C hardware_i2c;

/******************************************************************************
function: I2C device initialization
parameter:
    i2c_device : Device name
Info:   /dev/i2c-*
******************************************************************************/
void DEV_HARDWARE_I2C_begin(char *i2c_device)
{
    //device
    if((hardware_i2c.fd = open(i2c_device, O_RDWR)) < 0)  { //打开I2C 
        perror("Failed to open i2c device.\n");  
        printf("Failed to open i2c device\r\n");
        exit(1); 
    } else {
        DEV_HARDWARE_I2C_Debug("open : %s\r\n", i2c_device);
    }
}

/******************************************************************************
function: I2C device End
parameter:
Info:
******************************************************************************/
void DEV_HARDWARE_I2C_end(void)
{
    if (close(hardware_i2c.fd) != 0){
        perror("Failed to close i2c device.\n");  
    }
}

/******************************************************************************
function: Set the device address for I2C access
parameter:
    addr : Device address accessed by I2C
Info:
******************************************************************************/
void DEV_HARDWARE_I2C_setSlaveAddress(uint8_t addr)
{
    if(ioctl(hardware_i2c.fd, I2C_SLAVE, addr) < 0)  {  
        printf("Failed to access bus.\n");  
        exit(1);  
    }
}

/******************************************************************************
function:   I2C Send data
parameter:
    buf  : Send data buffer address
    len  : Send data length
Info:
******************************************************************************/
uint8_t DEV_HARDWARE_I2C_write(const char * buf, uint32_t len)
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
uint8_t DEV_HARDWARE_I2C_read(uint8_t reg, char* buf, uint32_t len)
{
    uint8_t temp[1] = {reg};
    write(hardware_i2c.fd, temp, 1); 
    read(hardware_i2c.fd, buf, len);
    return 0;
}