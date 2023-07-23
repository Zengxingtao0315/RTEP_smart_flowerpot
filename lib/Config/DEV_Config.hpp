#ifndef _DEV_CONFIG_HPP_
#define _DEV_CONFIG_HPP_
#ifdef USE_DEV_LIB
    #include "RPI_sysfs_gpio.hpp"
    #include "dev_hardware_SPI.hpp"
    #include "dev_hardware_i2c.hpp"   
#endif

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <thread>
#include <chrono>
#include <iomanip>
extern "C" {
    #include <unistd.h>
}

#include "Debug.hpp"

#define USE_SPI 		1
#define USE_IIC 		0
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


/*------------------------------------------------------------------------------------------------------*/
class DEV {
	public:
		UBYTE ModuleInit(void);
		void  ModuleExit(void);

		void GPIO_Mode(UWORD Pin, UWORD Mode);
		void GPIO_Init(void);
		void Digt_Write(UWORD Pin, UBYTE Value);
		UBYTE Digt_Read(UWORD Pin);
		void Delay_ms(UDOUBLE xms);

		void I2C_Write_Byte(uint8_t value, uint8_t Cmd);
		void SPI_WriteByte(UBYTE Value);
		void SPI_Write_nByte(uint8_t *pData, uint32_t Len);
};
#endif
