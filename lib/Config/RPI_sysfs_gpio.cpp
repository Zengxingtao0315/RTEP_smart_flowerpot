/*****************************************************************************
* | File        :   SYSFS_GPIO.c
* | Author      :   reference
* | Function    :   Drive SYSFS_ GPIO
* | Info        :   Read and write /sys/class/gpio

******************************************************************************/
extern "C" {
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
}
#include "RPI_sysfs_gpio.hpp"
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


int GPIO::SYSFS_GPIO_Export(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Export Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    write(fd, buffer, len);
    
    SYSFS_GPIO_Debug( "Export: Pin%d\r\n", Pin);

    close(fd);
    return 0;
}

int GPIO::SYSFS_GPIO_Unexport(int Pin)
{
    char buffer[NUM_MAXBUF];
    int len;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "unexport Failed: Pin%d\n", Pin);
        return -1;
    }

    len = snprintf(buffer, NUM_MAXBUF, "%d", Pin);
    write(fd, buffer, len);
    
    SYSFS_GPIO_Debug( "Unexport: Pin%d\r\n", Pin);
    
    close(fd);
    return 0;
}

int GPIO::SYSFS_GPIO_Direction(int Pin, int Dir)
{
    const char dir_str[]  = "in\0out";
    char path[DIR_MAXSIZ];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/direction", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Set Direction failed: Pin%d\n", Pin);
        return -1;
    }

    if (write(fd, &dir_str[Dir == SYSFS_GPIO_IN ? 0 : 3], Dir == SYSFS_GPIO_IN ? 2 : 3) < 0) {
        SYSFS_GPIO_Debug("failed to set direction!\r\n");
        return -1;
    }

    if(Dir == SYSFS_GPIO_IN){
        SYSFS_GPIO_Debug("Pin%d:intput\r\n", Pin);
    }else{
        SYSFS_GPIO_Debug("Pin%d:Output\r\n", Pin);
    }
    
    close(fd);
    return 0;
}

int GPIO::SYSFS_GPIO_Read(int Pin)
{
    char path[DIR_MAXSIZ];
    char value_str[3];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Read failed Pin%d\n", Pin);
        return -1;
    }

    if (read(fd, value_str, 3) < 0) {
        SYSFS_GPIO_Debug( "failed to read value!\n");
        return -1;
    }

    close(fd);
    return(atoi(value_str));
}

int GPIO::SYSFS_GPIO_Write(int Pin, int value)
{
    const char s_values_str[] = "01";
    char path[DIR_MAXSIZ];
    int fd;
    
    snprintf(path, DIR_MAXSIZ, "/sys/class/gpio/gpio%d/value", Pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        SYSFS_GPIO_Debug( "Write failed : Pin%d,value = %d\n", Pin, value);
        return -1;
    }

    if (write(fd, &s_values_str[value == SYSFS_GPIO_LOW ? 0 : 1], 1) < 0) {
        SYSFS_GPIO_Debug( "failed to write value!\n");
        return -1;
    }
    
    close(fd);
    return 0;
}
