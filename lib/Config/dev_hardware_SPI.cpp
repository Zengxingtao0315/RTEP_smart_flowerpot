/*****************************************************************************

This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#include "dev_hardware_SPI.hpp"




using namespace std;

/******************************************************************************
function:   SPI port initialization
parameter:
    SPI_device : Device name
Info:
    /dev/spidev0.0 
    /dev/spidev0.1
******************************************************************************/

void DEV_SPI::DEV_HARDWARE_SPI_beginSet(char *SPI_device, SPIMode mode, uint32_t speed)
{
    //device
    int ret = 0; 
    hardware_SPI.mode = 0;
    if((hardware_SPI.fd = open(SPI_device, O_RDWR )) < 0)  {
        std::cerr << "ailed to open SPI device" << std::endl;
        std::exit(1); 
    } else {
        DEV_HARDWARE_SPI_Debug("open : %s\r\n", SPI_device);
    }
    
    ret = ioctl(hardware_SPI.fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) 
        DEV_HARDWARE_SPI_Debug("can't set bits per word\r\n"); 
 
    ret = ioctl(hardware_SPI.fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) 
        DEV_HARDWARE_SPI_Debug("can't get bits per word\r\n"); 

    DEV_HARDWARE_SPI_Mode(mode);
    DEV_HARDWARE_SPI_ChipSelect(SPI_CS_Mode_LOW);
    DEV_HARDWARE_SPI_setSpeed(speed);
    DEV_HARDWARE_SPI_SetDataInterval(0);
}


/******************************************************************************
function:   SPI device End
parameter:
Info:
******************************************************************************/
void DEV_SPI::DEV_HARDWARE_SPI_end(void)
{
    hardware_SPI.mode = 0;
    if (close(hardware_SPI.fd) != 0){
        DEV_HARDWARE_SPI_Debug("Failed to close SPI device\r\n");
        perror("Failed to close SPI device.\n");  
    }
}

/******************************************************************************
function:   Set SPI speed
parameter:
Info:   Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_SPI::DEV_HARDWARE_SPI_setSpeed(uint32_t speed)
{
    uint32_t speed1 = hardware_SPI.speed;
    
    hardware_SPI.speed = speed;

    //Write speed
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        DEV_HARDWARE_SPI_Debug("can't set max speed hz\r\n"); 
        hardware_SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    
    //Read the speed of just writing
    if (ioctl(hardware_SPI.fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1) {
        DEV_HARDWARE_SPI_Debug("can't get max speed hz\r\n"); 
        hardware_SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    hardware_SPI.speed = speed;
    tr.speed_hz = hardware_SPI.speed;
    return 1;
}

/******************************************************************************
function:   Set SPI Mode
parameter:
Info:  
    SPIMode:
        SPI_MODE0 
        SPI_MODE1 
        SPI_MODE2 
        SPI_MODE3
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_SPI::DEV_HARDWARE_SPI_Mode(SPIMode mode)
{
    hardware_SPI.mode &= 0xfC;//Clear low 2 digits
    hardware_SPI.mode |= mode;//Setting mode
    
    //Write device
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        DEV_HARDWARE_SPI_Debug("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}

/******************************************************************************
function:   Chip Select
parameter:
Info:  
    CS_Mode:
        SPI_CS_Mode_LOW
        SPI_CS_Mode_HIGH
        SPI_CS_Mode_NONE
    Return :
        Return 1 success 
        Return -1 failed
******************************************************************************/
int DEV_SPI::DEV_HARDWARE_SPI_ChipSelect(SPIChipSelect CS_Mode)
{
    if(CS_Mode == SPI_CS_Mode_HIGH){
        hardware_SPI.mode |= SPI_CS_HIGH;
        hardware_SPI.mode &= ~SPI_NO_CS;
        DEV_HARDWARE_SPI_Debug("CS HIGH \r\n");
    }else if(CS_Mode == SPI_CS_Mode_LOW){
        hardware_SPI.mode &= ~SPI_CS_HIGH;
        hardware_SPI.mode &= ~SPI_NO_CS;
    }else if(CS_Mode == SPI_CS_Mode_NONE){
        hardware_SPI.mode |= SPI_NO_CS;
    }
    
    if (ioctl(hardware_SPI.fd, SPI_IOC_WR_MODE, &hardware_SPI.mode) == -1) {
        DEV_HARDWARE_SPI_Debug("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}



/******************************************************************************
function: 
    Time interval after transmission of one byte during continuous transmission
parameter:
    us :   Interval time (us)
Info:
******************************************************************************/
void DEV_SPI::DEV_HARDWARE_SPI_SetDataInterval(uint16_t us)
{
    hardware_SPI.delay = us;
    tr.delay_usecs  = hardware_SPI.delay;
}

/******************************************************************************
function: SPI port sends one byte of data
parameter:
    buf :   Sent data
Info:
******************************************************************************/
uint8_t DEV_SPI::DEV_HARDWARE_SPI_TransferByte(uint8_t buf)
{
    uint8_t rbuf[1];
    tr.len = 1;
    tr.tx_buf =  (unsigned long)&buf;
    tr.rx_buf =  (unsigned long)rbuf;
    
    //ioctl Operation, transmission of data
    if ( ioctl(hardware_SPI.fd, SPI_IOC_MESSAGE(1), &tr) < 1 ) {  
        DEV_HARDWARE_SPI_Debug("can't send spi message\r\n"); 
		return -1;
	}
    return rbuf[0];
}

/******************************************************************************
function: The SPI port reads a byte
parameter:
Info: Return read data
******************************************************************************/
int DEV_SPI::DEV_HARDWARE_SPI_Transfer(uint8_t *buf, uint32_t len)
{
    tr.len = len;
    tr.tx_buf =  (unsigned long)buf;
    tr.rx_buf =  (unsigned long)buf;
    
    //ioctl Operation, transmission of data
    if (ioctl(hardware_SPI.fd, SPI_IOC_MESSAGE(1), &tr)  < 1 ){  
        DEV_HARDWARE_SPI_Debug("can't send spi message\r\n"); 
        return -1;
    }
    
    return 1;
}

