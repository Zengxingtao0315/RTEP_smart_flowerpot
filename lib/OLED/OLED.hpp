#ifndef __OLED_HPP_
#define __OLED_HPP_

#include <cstdint> // C++ standard header file that includes definitions of integer types like uint8_t and uint16_t.
#include <cstring> // C++ standard header file that includes the definition of memset function.
#include <cstdio>  // C++ standard header file that includes the definition of printf function.
#include <cstdlib> // C++ standard header file that includes the definition of exit function.
#include <ctime>
#include <wiringPi.h>
#include <iostream> // C++ standard header file that includes the definition of cout and endl.

extern "C" {
    #include <sys/socket.h>
    #include <netdb.h>
    #include <netinet/in.h>
}

#include "../Config/DEV_Config.hpp"

#define OLED_WIDTH   128 // OLED width
#define OLED_HEIGHT  128 // OLED height

/********************************************************************************
Function:
    Define the full screen height length of the display
********************************************************************************/
class OLED {
public:
    OLED(DEV * devPtr);
    ~OLED();

    // Function to clear the OLED display.
    void Clear(void);

    // Function to display an image on the OLED.
    void Display(UBYTE *Image);

    // Function to set a specific window on the display and display an image in that window.
    void SetWindow_Display(uint8_t *Image, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    bool checkInit();

private:
    
    // Function to initialize the OLED display.
    void Init();
    // Function to reset the OLED.
    void Reset();

    // Function to write a command to the OLED.
    void WriteReg(uint8_t Reg);

    // Function to write data to the OLED.
    void WriteData(uint8_t Data);

    // Function to initialize the OLED registers.
    void InitReg();
    bool initSuccess = false;
    DEV * dev ;
};


#endif
