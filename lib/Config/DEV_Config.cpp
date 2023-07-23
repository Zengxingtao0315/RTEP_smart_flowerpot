/*****************************************************************************
* | File      	:   DEV_Config.cpp
* | Author      :   Xingtao Zeng
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

void DEV::Digt_Write(UWORD Pin, UBYTE Value)
{
#ifdef USE_DEV_LIB
    GPIO.SYSFS_GPIO_Write(Pin, Value);
    
#endif
}

UBYTE DEV::Digt_Read(UWORD Pin)
{
    UBYTE Read_value = 0;

#ifdef USE_DEV_LIB
    Read_value = GPIO.SYSFS_GPIO_Read(Pin);
#endif
    return Read_value;
}

void DEV::GPIO_Mode(UWORD Pin, UWORD Mode)
{
#ifdef USE_DEV_LIB
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
#ifdef USE_DEV_LIB
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
 /***********************
 #ifdef USE_BCM2835_LIB
    if(!bcm2835_init()) {
        std::cout << "bcm2835 init failed  !!!" << std::endl;
        return 1;
    } else {
		std::cout << "bcm2835 init success !!!" << std::endl;
    }
	GPIO_Init();
    #if USE_SPI
		std::cout << "USE_SPI" << std::endl;
        bcm2835_spi_begin();                                         //Start spi interface, set spi pin for the reuse function
        bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //High first transmission
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);                  //spi mode 3
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);  //Frequency
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //set CE0
        bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //enable cs0
    #elif USE_IIC
        Digt_Write(OLED_DS,0);
        Digt_Write(OLED_CS,0);
		std::cout << "USE_IIC" << std::endl;
        bcm2835_i2c_begin();	
        bcm2835_i2c_setSlaveAddress(0x3c);
        
    #endif
    
#elif USE_WIRINGPI_LIB  
    //if(wiringPiSetup() < 0) {//use wiringpi Pin number table  
    if(wiringPiSetupGpio() < 0) { //use BCM2835 Pin number table
		std::cout << "Set wiringPi lib failed	!!!" << std::endl;
        return 1;
    } else {
		std::cout << "set wiringPi lib success  !!!" << std::endl;
    }
	GPIO_Init();
    #if USE_SPI
        std::cout << "USE_SPI" << std::endl;      
        //wiringPiSPISetup(0,9000000);
        wiringPiSPISetupMode(0, 9000000, 3);
    #elif USE_IIC
        Digt_Write(OLED_DS,0);
        Digt_Write(OLED_CS,0);
        std::cout << "USE_IIC" << std::endl;
        fd = wiringPiI2CSetup(0x3c);
    #endif
   
#elif 
*******************/
#ifdef USE_DEV_LIB
	GPIO_Init();
    #if USE_SPI
        std::cout << "USE_SPI" << std::endl;      
        DEV_SPI.DEV_HARDWARE_SPI_beginSet("/dev/spidev0.0",mode,10000000);
    #elif USE_IIC   
        std::cout << "USE_IIC" << std::endl;	
        Digt_Write(OLED_DS,0)
        Digt_Write(OLED_CS,0);
        DEV_I2C.DEV_HARDWARE_I2C_begin("/dev/i2c-1");
        DEV_I2C.DEV_HARDWARE_I2C_setSlaveAddress(0x3c);
    #endif
#endif
    return 0;
}

void DEV::SPI_WriteByte(uint8_t Value)
{
    
#ifdef USE_DEV_LIB
	// printf("write data is %d\r\n", Value);
    DEV_SPI.DEV_HARDWARE_SPI_TransferByte(Value);
    
#endif
}

void DEV::SPI_Write_nByte(uint8_t *pData, uint32_t Len)
{
	std::cout << "data is " << pData;
#ifdef USE_DEV_LIB
    DEV_SPI.DEV_HARDWARE_SPI_Transfer(pData, Len);
    
#endif
}

void DEV::I2C_Write_Byte(uint8_t value, uint8_t Cmd)
{
#ifdef USE_DEV_LIB
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
#ifdef  USE_DEV_LIB
    Digt_Write(OLED_CS,0);
	Digt_Write(OLED_RST,1);
	Digt_Write(OLED_DC,0);
    DEV_SPI.DEV_HARDWARE_SPI_end();
    DEV_I2C.DEV_HARDWARE_I2C_end();
#endif
}

