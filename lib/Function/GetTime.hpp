#ifndef _GETTIME_HPP_
#define _GETTIME_HPP_

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
#include "sensor.hpp"


class Time {
public:
    PAINT_TIME getLocalTime();
};

class SunlightDurationRecorder {
public:
	float getSunlightDurationInHours();
private:
    bool initialized_{false};
    std::chrono::system_clock::time_point start_time_;
    std::chrono::duration<int, std::ratio<1, 1>> duration_0_{0};
    std::chrono::system_clock::time_point last_reset_time_;
};

#endif