/*****************************************************************************

This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#include "SPI.hpp"




using namespace std;

/**
 * Function: Initialize the hardware SPI communication.
 * 
 * param SPI_device : Path to the SPI device.
 * param mode       : SPI mode (SPIMode).
 * param speed      : SPI communication speed in H
 * 
 */

void DEV_SPI::DEV_SPI_beginSet(char *SPI_device, SPIMode mode, uint32_t speed)
{
    //device
    int ret = 0; 
    SPI.mode = 0;
    if((SPI.fd = open(SPI_device, O_RDWR )) < 0)  {
        std::cerr << "ailed to open SPI device" << std::endl;
        std::exit(1); 
    } else {
        Debug("open : %s\r\n", SPI_device);
    }
    
    ret = ioctl(SPI.fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1) 
        Debug("can't set bits per word\r\n"); 
 
    ret = ioctl(SPI.fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1) 
        Debug("can't get bits per word\r\n"); 

    DEV_SPI_Mode(mode);
    DEV_SPI_ChipSelect(SPI_CS_Mode_LOW);
    DEV_SPI_setSpeed(speed);
    DEV_SPI_SetDataInterval(0);
}


/**
 * Function: End the hardware SPI communication.
 */
void DEV_SPI::DEV_SPI_end(void)
{
    SPI.mode = 0;
    if (close(SPI.fd) != 0){
        Debug("Failed to close SPI device\r\n");
        perror("Failed to close SPI device.\n");  
    }
}

/**
 * Function: Set the speed of the hardware SPI communication.
 * 
 * param speed - The desired speed in Hz.
 * return Returns 1 if successful, -1 if there was an error.
 */
int DEV_SPI::DEV_SPI_setSpeed(uint32_t speed)
{
    uint32_t speed1 = SPI.speed;
    
    SPI.speed = speed;

    //Write speed
    if (ioctl(SPI.fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        Debug("can't set max speed hz\r\n"); 
        SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    
    //Read the speed of just writing
    if (ioctl(SPI.fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1) {
        Debug("can't get max speed hz\r\n"); 
        SPI.speed = speed1;//Setting failure rate unchanged
        return -1;
    }
    SPI.speed = speed;
    tr.speed_hz = SPI.speed;
    return 1;
}

/**
 * Function: Set the SPI mode of the hardware SPI communication.
 * 
 * param mode - The desired SPI mode (0-3).
 * return Returns 1 if successful, -1 if there was an error.
 */
int DEV_SPI::DEV_SPI_Mode(SPIMode mode)
{
    SPI.mode &= 0xfC;//Clear low 2 digits
    SPI.mode |= mode;//Setting mode
    
    //Write device
    if (ioctl(SPI.fd, SPI_IOC_WR_MODE, &SPI.mode) == -1) {
        Debug("can't set spi mode\r\n"); 
        return -1;
    }
    return 1;
}

/**
 * Function: Set the SPI chip select mode for hardware SPI communication.
 * 
 * param CS_Mode - The desired chip select mode (SPI_CS_Mode_HIGH, SPI_CS_Mode_LOW, SPI_CS_Mode_NONE).
 * return Returns 1 if successful, -1 if there was an error.
 */
int DEV_SPI::DEV_SPI_ChipSelect(SPIChipSelect CS_Mode)
{
    // Check the provided CS_Mode and configure the SPI mode accordingly
    if(CS_Mode == SPI_CS_Mode_HIGH){
        SPI.mode |= SPI_CS_HIGH;  // Set CS pin to high
        SPI.mode &= ~SPI_NO_CS;  // Enable CS
        Debug("CS HIGH \r\n");  // Print debug message
    } else if(CS_Mode == SPI_CS_Mode_LOW){
        SPI.mode &= ~SPI_CS_HIGH;  // Set CS pin to low
        SPI.mode &= ~SPI_NO_CS;   // Enable CS
    } else if(CS_Mode == SPI_CS_Mode_NONE){
        SPI.mode |= SPI_NO_CS;   // Disable CS (no chip select)
    }
    
    // Try to set the SPI mode using ioctl
    if (ioctl(SPI.fd, SPI_IOC_WR_MODE, &SPI.mode) == -1) {
        Debug("can't set spi mode\r\n"); // Print error message
        return -1;  // Return -1 to indicate an error
    }
    
    return 1;  // Return 1 to indicate success
}




/**
 * Function: Set the data interval for hardware SPI communication.
 * 
 * param us - The data interval in microseconds.
 */
void DEV_SPI::DEV_SPI_SetDataInterval(uint16_t us)
{
    SPI.delay = us;
    tr.delay_usecs  = SPI.delay;
}

/**
 * Function: Transfer a byte using hardware SPI.
 * 
 * param buf - The byte to be transferred.
 * return The received byte.
 */
uint8_t DEV_SPI::DEV_SPI_TransferByte(uint8_t buf)
{
    uint8_t rbuf[1];  // Initialize a buffer to hold the received byte

    // Configure the transfer structure
    tr.len = 1;  // Set the length of the transfer
    tr.tx_buf =  (unsigned long)&buf;  // Set the transmit buffer to the address of 'buf'
    tr.rx_buf =  (unsigned long)rbuf;  // Set the receive buffer to the address of 'rbuf'

    // Perform the SPI transfer using ioctl
    if (ioctl(SPI.fd, SPI_IOC_MESSAGE(1), &tr) < 1) {  
        Debug("can't send spi message\r\n");  // Print an error message
        return -1;  // Return -1 to indicate an error
    }

    return rbuf[0];  // Return the received byte
}


/**
 * Function: Transfer data using hardware SPI.
 * 
 * param buf - Pointer to the data to be transferred.
 * param len - Length of the data in bytes.
 * return 1 if successful, -1 on error.
 */
int DEV_SPI::DEV_SPI_Transfer(uint8_t *buf, uint32_t len)
{
    // Configure the transfer structure
    tr.len = len;        // Set the length of the transfer
    tr.tx_buf = (unsigned long)buf;  // Set the transmit buffer to the address of 'buf'
    tr.rx_buf = (unsigned long)buf;  // Set the receive buffer to the address of 'buf'

    // Perform the SPI transfer using ioctl
    if (ioctl(SPI.fd, SPI_IOC_MESSAGE(1), &tr) < 1) {  
        Debug("can't send spi message\r\n");  // Print an error message
        return -1;  // Return -1 to indicate an error
    }
    
    return 1;  // Return 1 to indicate success
}


