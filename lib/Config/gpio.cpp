/*****************************************************************************

This code is adapted from the 2-CH-RS485-HAT project by waveshare, licensed under the MIT license.
// Original source: https://github.com/waveshare/2-CH-RS485-HAT/tree/master/RaspberryPi/user_dev/c/lib/Config
// Porject is Copyright (c) 2023 Xingtao Zeng, all rights reserved.
// Project is licensed under the MIT License: https://opensource.org/licenses/MIT
******************************************************************************/
extern "C" {
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
}
#include "gpio.hpp"
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/**
 * Function: Export a GPIO pin using sysfs.
 * 
 * param Pin - GPIO pin number to be exported.
 * return 0 if successful, -1 on error.
 */
int GPIO::SYSFS_GPIO_Export(int Pin)
{
    char buffer[NUM_MAXBUF];  
    int len;  
    int fd;  

    // Open the export file for writing
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        Debug("Export Failed: Pin%d\n", Pin);
        return -1;  // Return -1 to indicate an error
    }

    // Convert the pin number to a string and write it to the export file
    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    write(fd, buffer, len);
    
    Debug("Export: Pin%d\r\n", Pin);  

    
    close(fd);
    return 0;  
}


/**
 * Function: Set the direction of a GPIO pin using sysfs.
 * 
 * param Pin - GPIO pin number to set the direction for.
 * param Dir - Direction to set (GPIO_IN for input, GPIO_OUT for output).
 * return 0 if successful, -1 on error.
 */
int GPIO::SYSFS_GPIO_Direction(int Pin, int Dir)
{
    const char dir_str[] = "in\0out";  
    char path[DIR_MAXSIZ];  
    int fd;  
    
    // Generate the path to the GPIO direction file
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/direction", Pin);
    
    // Open the file for writing
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        Debug("Set Direction failed: Pin%d\n", Pin);
        return -1;  
    }

    // Write the direction (either "in" or "out") to the GPIO direction file
    if (write(fd, &dir_str[Dir == GPIO_IN ? 0 : 3], Dir == GPIO_IN ? 2 : 3) < 0) {
        Debug("failed to set direction!\r\n");
        close(fd);
        return -1;  
    }

    // Print debug message indicating the set direction
    if (Dir == GPIO_IN) {
        Debug("Pin%d: Input\r\n", Pin);
    } else {
        Debug("Pin%d: Output\r\n", Pin);
    }
    
   
    close(fd);
    
    return 0;  
}


/**
 * Function: Write a value to a GPIO pin using sysfs.
 * 
 * param Pin - GPIO pin number to write to.
 * param value - Value to write (GPIO_LOW for 0, GPIO_HIGH for 1).
 * return 0 if successful, -1 on error.
 */
int GPIO::SYSFS_GPIO_Write(int Pin, int value)
{
    const char s_values_str[] = "01";  
    char path[DIR_MAXSIZ];  
    int fd; 
    
    // Generate the path to the GPIO value file
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    
    // Open the file for writing
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        Debug("Write failed : Pin%d,value = %d\n", Pin, value);
        return -1;  
    }

    // Write the value (0 or 1) to the GPIO file
    if (write(fd, &s_values_str[value == GPIO_LOW ? 0 : 1], 1) < 0) {
        Debug("failed to write value!\n");
        close(fd);
        return -1;  
    }
    
  
    close(fd);
    
    return 0;  
}

