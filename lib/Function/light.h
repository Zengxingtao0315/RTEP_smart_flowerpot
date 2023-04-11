#include <cstdio>
#include <wiringPi.h>
#include <iostream>

#define DIGITAL_PIN 0   // 数字输出引脚
#define ANALOG_PIN  1   // 模拟输出引脚



int readDigitalValue();
int readAnalogValue();