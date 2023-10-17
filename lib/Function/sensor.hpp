#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_
#include <fstream>
#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include "../Config/DEV_Config.hpp"
#include "Plant.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


const int MAXTIMINGS = 85;
const int DHTPIN = 7;
const int DIGITALPIN = 1;
  
typedef struct DHTdata {
    double temperature;
    double humidity;
};

class Sensor {
private:
    int digitalPin;
    int dhtPin;
	double temperature = 0.0;
	double humidity = 0.0;
	std::thread dhtThread;  
    std::mutex dataMutex;   
	void readDHTdataLoop();
	double lastTemperature = 0.0;
    double lastHumidity = 0.0;
	std::condition_variable dataCondVar;
    std::atomic<bool> isThreaedRunning;
public:
    Sensor(int digitalPin, int dhtPin);
	~Sensor();
	double getTemperature();

	double getHumidity();
    UWORD readDigitalValue();
    //UWORD readAnalogValue();
    DHTdata readDHTdata();
	void sendDHTdataToHTML(const DHTdata& data);
};


#endif
