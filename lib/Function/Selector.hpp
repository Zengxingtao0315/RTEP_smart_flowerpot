#ifndef _SELECTOR_HPP_
#define _SELECTOR_HPP_

#include <iostream>
#include <cstring>
#include <string>
#include "checkInternet.hpp"
#include "sensor.hpp"
#include "GetTime.hpp"
#include "../Config/DEV_Config.hpp"
#include "Plant.hpp"

class Selector {
public:
    const char* EmojiSelector(double temperature, double humidity);
    const char* BmpSelector();
	const char* lightSelector();
	
	void setTemperatureRange(range tRg);
	void setHumidityRange(range hRg);
	void setLightRatioRange(float lr);


	Selector(Sensor * sensor, range tRg, range hRg, range lrRg);
	~Selector();
	
private:
	char* bmp_path = "./pic/internet_down.bmp";
	bool light_flag = 0;
	Sensor * sensorPtr = new sensor(DIGITALPIN, DHTPIN);
	SunlightDurationRecorder LightRecorder = new SunlightDurationRecorder(sensorPtr);
	range tempRange;
	range humiRange;
	range lightratiorange;

};


#endif