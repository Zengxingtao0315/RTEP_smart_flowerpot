/*****************************************************************************

This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#ifndef __SPI_HPP_
#define __SPI_HPP_

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <cstdint> 
#include <cstdio> 
#include <cstdlib> 
#include <iostream>
#include "Debug.hpp"
extern "C" {
	#include <getopt.h> 
	#include <unistd.h> 
	#include <fcntl.h> 
	#include <sys/ioctl.h> 
	#include <linux/types.h> 
	#include <linux/spi/spidev.h> 
}



#define SPI_CPHA        0x01
#define SPI_CPOL        0x02
#define SPI_MODE_0      (0|0)
#define SPI_MODE_1      (0|SPI_CPHA)
#define SPI_MODE_2      (SPI_CPOL|0)
#define SPI_MODE_3      0x03
#define SPI_CS_HIGH     0x04                //Chip select high  
#define SPI_LSB_FIRST   0x08                //LSB  
#define SPI_3WIRE       0x10                //3-wire mode SI and SO same line
#define SPI_LOOP        0x20                //Loopback mode  
#define SPI_NO_CS       0x40                //A single device occupies one SPI bus, so there is no chip select 
#define SPI_READY       0x80                //Slave pull low to stop data transmission  



typedef enum{
    SPI_MODE0 = SPI_MODE_0,  /*!< CPOL = 0, CPHA = 0 */
    SPI_MODE1 = SPI_MODE_1,  /*!< CPOL = 0, CPHA = 1 */
    SPI_MODE2 = SPI_MODE_2,  /*!< CPOL = 1, CPHA = 0 */
    SPI_MODE3 = SPI_MODE_3   /*!< CPOL = 1, CPHA = 1 */
}SPIMode;


typedef enum{
    SPI_CS_Mode_LOW  = 0,     /*!< Chip Select 0 */
    SPI_CS_Mode_HIGH = 1,     /*!< Chip Select 1 */
    SPI_CS_Mode_NONE = 3  /*!< No CS, control it yourself */
}SPIChipSelect;



typedef enum
{
    SPI_3WIRE_Mode = 0,
    SPI_4WIRE_Mode = 1
}BusMode;


/**
 * Define SPI attribute
**/
typedef struct SPIStruct {
    //GPIO
    uint16_t SCLK_PIN;
    uint16_t MOSI_PIN;
    uint16_t MISO_PIN;
    
    uint16_t CS0_PIN;
    uint16_t CS1_PIN;
    
    
    uint32_t speed;
    uint16_t mode;
    uint16_t delay;
    int fd; //
};

static uint8_t bits = 8; 




class DEV_SPI {
	private:
        int DEV_SPI_setSpeed(uint32_t speed);
		int DEV_SPI_Mode(SPIMode mode);
        int DEV_SPI_ChipSelect(SPIChipSelect CS_Mode);
        void DEV_SPI_SetDataInterval(uint16_t us);
        
        struct SPIStruct SPI;
        struct spi_ioc_transfer tr;

    public:
		void DEV_SPI_beginSet(char *SPI_device, SPIMode mode, uint32_t speed);
		void DEV_SPI_end(void);
		uint8_t DEV_SPI_TransferByte(uint8_t buf);
		int DEV_SPI_Transfer(uint8_t *buf, uint32_t len);

};

#endif