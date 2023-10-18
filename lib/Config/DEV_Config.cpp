/*****************************************************************************
This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
#include "DEV_Config.hpp"
extern "C" {
	#include <unistd.h>
	}
#include <iostream> 


using namespace std;
/*****************************************
                GPIO
*****************************************/
void DEV::Digital_Write(UWORD Pin, UBYTE Value)
{

    gpio.GPIO_Write(Pin, Value);
    

}

/**
 * Function: Set the mode of a GPIO pin.
 * 
 * param Pin   : GPIO pin number.
 * param Mode  : Desired mode for the pin (0 or SYSFS_GPIO_IN for input, any other value for output).
 */
void DEV::GPIO_Mode(UWORD Pin, UWORD Mode)
{
    gpio.GPIO_Export(Pin);
    if(Mode == 0 || Mode == GPIO_IN){
        gpio.GPIO_Direction(Pin, GPIO_IN);
        // printf("IN Pin = %d\r\n",Pin);
    }else{
        gpio.GPIO_Direction(Pin, GPIO_OUT);
        // printf("OUT Pin = %d\r\n",Pin);
    }

}

/**
 * delay x ms
**/
void DEV::Delay_ms(UDOUBLE xms)
{
    
    std::this_thread::sleep_for(std::chrono::milliseconds(xms));


}

void DEV::GPIO_Init(void)
{
    GPIO_Mode(OLED_CS, 1);
    GPIO_Mode(OLED_RST, 1);
    GPIO_Mode(OLED_DC, 1);
}

/**
 * Function: module intialization
 */
UBYTE DEV::ModuleInit(void)
{
	SPIMode mode = SPI_MODE3;
	GPIO_Init();
    std::cout << "USE_SPI" << std::endl;      
    dev_spi.DEV_SPI_beginSet("/dev/spidev0.0",mode,10000000);
    return 0;
}



void DEV::SPI_WriteByte(UBYTE Value)
{

	// printf("write data is %d\r\n", Value);
    dev_spi.DEV_SPI_TransferByte(Value);

}


/**
 * Function: modile exit in a normal way
 */
void DEV::ModuleExit(void)
{

    Digital_Write(OLED_CS,0);
	Digital_Write(OLED_RST,1);
	Digital_Write(OLED_DC,0);
    dev_spi.DEV_SPI_end();

}

