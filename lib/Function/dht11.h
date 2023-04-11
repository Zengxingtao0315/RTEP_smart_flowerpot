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
#define DIGITAL_PIN 0   // 数字输出引脚
#define ANALOG_PIN  1   // 模拟输出引脚


typedef struct {
    uint8_t humidity_int;
    uint8_t humidity_dec;
    uint8_t temperature_int;
    uint8_t temperature_dec;
    uint8_t data[5];
} DHTData;


int read_dht_data(DHTData *dht_data);

int readDigitalValue();
int readAnalogValue();