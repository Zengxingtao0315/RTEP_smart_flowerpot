#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include "../Config/DEV_Config.hpp"


#define MAXTIMINGS  85
#define DHTPIN    7
#define DIGITALPIN 1   
  
struct {
	double temperature;
	double humidity;
}DHTdata;
class Sensor {
private:
    int digitalPin;
    //int analogPin;
    int dhtPin;

public:
    Sensor(int digitalPin, int dhtPin);
    UWORD readDigitalValue();
    //UWORD readAnalogValue();
    DHTdata readDHTdata();
};
#endif
