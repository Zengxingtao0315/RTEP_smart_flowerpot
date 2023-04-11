#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include "test.h"
#include <cstdio>
#include <wiringPi.h>
#include <iostream>


#define MAXTIMINGS  85
#define DHTPIN    7
#define DIGITALPIN 0   // 数字输出引脚
#define ANALOGPIN  3   // 模拟输出引脚


typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
    uint8_t data[5];
} DHTData;


class Sensor {
private:
    int digitalPin;
    int analogPin;
    int dhtPin;
    DHTData dht_data;

public:
    Sensor(int digitalPin, int analogPin, int dhtPin);
    int readDigitalValue();
    int readAnalogValue();
    int readDHTdata(float& temperature, float& humidity);
};
