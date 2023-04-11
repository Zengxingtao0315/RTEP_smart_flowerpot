#ifndef __OLED_1IN5_RGB_H
#define __OLED_1IN5_RGB_H

#include <cstdint> // C++标准头文件，其中包含uint8_t和uint16_t等整数类型的定义。
#include <cstring> // C++标准头文件，其中包含memset函数的定义。
#include <cstdio> // C++标准头文件，其中包含printf函数的定义。
#include <cstdlib> // C++标准头文件，其中包含exit函数的定义。
#include <sys/socket.h>
#include <netdb.h>
#include <ctime>
#include <wiringPi.h>
#include <netinet/in.h>
#include <iostream> // C++标准头文件，其中包含cout和endl的定义。

extern "C" {
    #include "DEV_Config.h"
}

#define OLED_WIDTH 128;//OLED width
#define OLED_HEIGHT  128;//OLED height
/********************************************************************************
function:	
		Define the full screen height length of the display
********************************************************************************/
class OLED {
public:
	void Init(void);
	void Clear(void);
	void Display(UBYTE *Image);
	void SetWindow_Display(uint8_t *Image, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

private:
    static void Reset();
    static void WriteReg(uint8_t Reg);
    static void WriteData(uint8_t Data);
    static void InitReg();
};

typedef struct {
	float temperature;
	float humidity;
	float light_intensity;
	} pSTATUS;




#endif  