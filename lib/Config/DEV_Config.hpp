#ifndef _DEV_CONFIG_HPP_
#define _DEV_CONFIG_HPP_
/***********************************************************************************************************************
			------------------------------------------------------------------------
			|\\\																///|
			|\\\					Hardware interface							///|
			------------------------------------------------------------------------
***********************************************************************************************************************/
#ifdef USE_BCM2835_LIB
    #include <bcm2835.h>
#elif USE_WIRINGPI_LIB
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
	#include <wiringPiI2C.h>
#elif USE_DEV_LIB
    #include "RPI_sysfs_gpio.hpp"
    #include "dev_hardware_SPI.hpp"
    #include "dev_hardware_i2c.hpp"   
#endif

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdint>
extern "C" {
    #include <unistd.h>
}

#include "Debug.hpp"

#define USE_SPI 1
#define USE_IIC 0
#define IIC_CMD        0X00
#define IIC_RAM        0X40


/**
 * data
**/
#define UBYTE   std::uint8_t
#define UWORD   std::uint16_t
#define UDOUBLE std::uint32_t

//OLED Define
#define OLED_CS         8		
#define OLED_RST        27	
#define OLED_DC         25	


#define OLED_CS_0      DEV_Digital_Write(OLED_CS,0)
#define OLED_CS_1      DEV_Digital_Write(OLED_CS,1)

#define OLED_RST_0      DEV_Digital_Write(OLED_RST,0)
#define OLED_RST_1      DEV_Digital_Write(OLED_RST,1)

#define OLED_DC_0       DEV_Digital_Write(OLED_DC,0)
#define OLED_DC_1       DEV_Digital_Write(OLED_DC,1)

/*------------------------------------------------------------------------------------------------------*/
class DEV {
	public:
		UBYTE ModuleInit(void);
		void  ModuleExit(void);

		void GPIO_Mode(UWORD Pin, UWORD Mode);
		void Digital_Write(UWORD Pin, UBYTE Value);
		UBYTE Digital_Read(UWORD Pin);
		void Delay_ms(UDOUBLE xms);

		void I2C_Write_Byte(uint8_t value, uint8_t Cmd);
		void SPI_WriteByte(UBYTE Value);
		void SPI_Write_nByte(uint8_t *pData, uint32_t Len);
};
#endif
