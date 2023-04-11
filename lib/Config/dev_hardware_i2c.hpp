/*****************************************************************************
* | File        :   dev_hardware_i2c.hpp
* | Author      :   Xingtao Zeng
* | Function    :   Read and write /dev/i2C,  hardware I2C
* | Info        :

******************************************************************************/
#ifndef __DEV_HARDWARE_I2C_HPP_
#define __DEV_HARDWARE_I2C_HPP_

#include <cstdint>


#define DEV_HARDWARE_I2C_DEBUG 0
#if DEV_HARDWARE_I2C_DEBUG
#define DEV_HARDWARE_I2C_Debug(__info,...) std::cout << "Debug: " << info << __VA_ARGS__ << std::endl
#else
#define DEV_HARDWARE_I2C_Debug(__info,...)
#endif

/**
 * Define I2C attribute
**/
typedef struct I2CStruct {
    //GPIO
    uint16_t SCL_PIN;
    uint16_t SDA_PIN;
    
    int fd; //I2C device file descriptor
    uint16_t addr; //I2C device address
} HARDWARE_I2C;

class DEV_I2C{
	public:
		void DEV_HARDWARE_I2C_begin(char *i2c_device);
		void DEV_HARDWARE_I2C_end(void);
		void DEV_HARDWARE_I2C_setSlaveAddress(uint8_t addr);
		uint8_t DEV_HARDWARE_I2C_write(const char * buf, uint32_t len);
		uint8_t DEV_HARDWARE_I2C_read(uint8_t reg, char* buf, uint32_t len);
};
#endif