#ifndef _RECODER_HPP_
#define _RECODER_HPP_

#include <wiringPi.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <chrono>
extern "C" {
	#include <time.h>
};
#include <ctime>
#include "../GUI/GUI_Paint.hpp"
#include "Sensor.hpp"
#include "Plant.hpp"
#include "GetTime.hpp"

class Recoder{
    private:
        float lightratio;
        Time Planttime;
        Sensor Light(DIGITALPIN,  DHTPIN);

    public:
        double DurationRecoder();
        double LightRecoder();
        double LightRatioRecoder();



}










#endif