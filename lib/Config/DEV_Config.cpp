/*****************************************************************************
* | File      	:   DEV_Config.cpp
* | Author      :   reference
* | Function    :   Hardware underlying interface
* | Info        :

******************************************************************************/
#include "DEV_Config.hpp"
extern "C" {
	#include <unistd.h>
	}
#include <iostream> 

uint32_t fd;
extern DEV_SPI DEV_SPI;
GPIO GPIO;
DEV_I2C DEV_I2C;
using namespace std;
/*****************************************
                GPIO
*****************************************/
void DEV::Digital_Write(UWORD Pin, UBYTE Value)
{
#if USE_DEV_LIB
    GPIO.SYSFS_GPIO_Write(Pin, Value);
    
#endif
}

UBYTE DEV::Digital_Read(UWORD Pin)
{
    UBYTE Read_value = 0;
#if USE_DEV_LIB
    Read_value = GPIO.SYSFS_GPIO_Read(Pin);
#endif
    return Read_value;
}

void DEV::GPIO_Mode(UWORD Pin, UWORD Mode)
{
#if USE_DEV_LIB
    GPIO.SYSFS_GPIO_Export(Pin);
    if(Mode == 0 || Mode == SYSFS_GPIO_IN){
        GPIO.SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_IN);
        // printf("IN Pin = %d\r\n",Pin);
    }else{
        GPIO.SYSFS_GPIO_Direction(Pin, SYSFS_GPIO_OUT);
        // printf("OUT Pin = %d\r\n",Pin);
    }
#endif   
}

/**
 * delay x ms
**/
void DEV::Delay_ms(UDOUBLE xms)
{
#if USE_DEV_LIB
    UDOUBLE i;
    
       std::this_thread::sleep_for(std::chrono::milliseconds(xms));

#endif
}

void DEV::GPIO_Init(void)
{
    GPIO_Mode(OLED_CS, 1);
    GPIO_Mode(OLED_RST, 1);
    GPIO_Mode(OLED_DC, 1);
}

/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV::ModuleInit(void)
{
	SPIMode mode = SPI_MODE3;
 #if USE_DEV_LIB
	GPIO_Init();
    #if USE_SPI
        std::cout << "USE_SPI" << std::endl;      
        DEV_SPI.DEV_HARDWARE_SPI_beginSet("/dev/spidev0.0",mode,10000000);
    #elif USE_IIC   
        std::cout << "USE_IIC" << std::endl;	
        Digital_Write(OLED_DS,0)
        Digital_Write(OLED_CS,0);
        DEV_I2C.DEV_HARDWARE_I2C_begin("/dev/i2c-1");
        DEV_I2C.DEV_HARDWARE_I2C_setSlaveAddress(0x3c);
    #endif
#endif
    return 0;
}

void DEV::SPI_WriteByte(uint8_t Value)
{
#if USE_DEV_LIB
	// printf("write data is %d\r\n", Value);
    DEV_SPI.DEV_HARDWARE_SPI_TransferByte(Value);
    
#endif
}

void DEV::SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
	std::cout << "data is " << pData;
#if USE_DEV_LIB
    DEV_SPI.DEV_HARDWARE_SPI_Transfer(pData, Len);
    
#endif
}

void DEV::I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
#if USE_DEV_LIB
    char wbuf[2]={Cmd, value};
    DEV_I2C.DEV_HARDWARE_I2C_write(wbuf, 2);

#endif
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
Info:
******************************************************************************/
void DEV::ModuleExit(void)
{
#if USE_DEV_LIB
    Digital_Write(OLED_CS,0);
	Digital_Write(OLED_RST,1);
	Digital_Write(OLED_DC,0);
    DEV_SPI.DEV_HARDWARE_SPI_end();
    DEV_I2C.DEV_HARDWARE_I2C_end();
#endif
}

